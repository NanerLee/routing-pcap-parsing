/**
* @Author: nanerlee
* @Date:   2017-02-20T16:38:25+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T15:37:44+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "pcap-file.h"

PcapFile::PcapFile(std::string file)
    : filename(file), linktype(0xFFFFFFFF), count(0)
{
}

void PcapFile::Parse()
{
    std::ifstream pcapfile;
    pcapfile.open(filename, std::ios::in | std::ios::binary);
    if (pcapfile.is_open())
    {
        pcapfile.read((char*)&fh, sizeof(PcapFileHeader));
        linktype = fh.linktype;
        while (pcapfile.peek() != EOF)
        {
            std::vector<std::shared_ptr<Packet>> temp_pkt;

            // pcap pcaket header
            auto pcapp            = std::make_shared<PcapPacket>(pcapfile);
            uint32_t total_length = pcapp->get_caplen();  // 一个数据包的总长度
            temp_pkt.push_back(pcapp);
            // L2
            if (linktype == 1)  //以太网
            {
                auto ethp    = std::make_shared<EthPacket>(pcapfile);
                total_length = total_length - sizeof(EthHeader);
                temp_pkt.push_back(ethp);
            }
            else if (linktype == 9)  // p2p
            {
                auto pppp    = std::make_shared<PppPacket>(pcapfile);
                total_length = total_length - sizeof(PppHeader);
                temp_pkt.push_back(pppp);
            }
            else
            {
                std::cout << "unknow linktype: " << linktype << '\n';
                pcapfile.seekg(total_length, std::ios::cur);
            }

            // L3
            auto ipv4p   = std::make_shared<Ipv4Packet>(pcapfile);
            total_length = total_length - ipv4p->get_tlen();
            temp_pkt.push_back(ipv4p);

            if (ipv4p->IsFragment())
            {
                packets.push_back(temp_pkt);
                HandleFragment(ipv4p, pcapfile);
                pcapfile.seekg(total_length, std::ios::cur);  //跳过以太网帧的fcs
                continue;
            }

            // L4
            if (ipv4p->get_protocol() == 17)  // udp
            {
                auto udpp = std::make_shared<UdpPacket>(pcapfile);
                temp_pkt.push_back(udpp);

                if (udpp->get_sport() == 520 && udpp->get_dport() == 520)  // rip
                {
                    auto ripp = std::make_shared<RipPacket>(
                        pcapfile, udpp->get_tlen() - sizeof(UdpHeader));
                    temp_pkt.push_back(ripp);
                }
                else
                {
                    pcapfile.seekg(sizeof(UdpHeader), std::ios::cur);
                }
            }
            else if (ipv4p->get_protocol() == 88)  // eigrp
            {
                auto eigrpp = std::make_shared<EigrpPacket>(
                    pcapfile, ipv4p->get_tlen() - sizeof(Ipv4Header));
                temp_pkt.push_back(eigrpp);
            }
            else if (ipv4p->get_protocol() == 89)  // ospf
            {
                auto ospfp = std::make_shared<OspfPacket>(
                    pcapfile, ipv4p->get_tlen() - sizeof(Ipv4Header));
                temp_pkt.push_back(ospfp);
            }
            else
            {
                std::cout << "unknow protocol: " << ipv4p->get_protocol() << '\n';
                pcapfile.seekg(ipv4p->get_tlen() - sizeof(Ipv4Header), std::ios::cur);
            }
            packets.push_back(temp_pkt);
            pcapfile.seekg(total_length, std::ios::cur);  //跳过以太网帧的fcs
        }
        count = packets.size();
        pcapfile.close();
    }
    else
    {
        std::cout << "Can't open file: " << filename << '\n';
        exit(-1);
    }
}

uint32_t PcapFile::get_count()
{
    return count;
}
std::vector<std::shared_ptr<Packet>>& PcapFile::get_packet(uint32_t i)
{
    return packets[i];
}
std::shared_ptr<Packet> PcapFile::get_packet(uint32_t i, uint32_t j)
{
    return packets[i][j];
}

void PcapFile::HandleFragment(std::shared_ptr<Ipv4Packet> ipv4p, std::istream& is)
{
    auto temp_iter = fragments.end();
    for (auto iter = fragments.begin(); iter != fragments.end(); ++iter)
    {
        if ((*iter)->IsSameIpPacket(ipv4p))
        {
            temp_iter = iter;
            break;
        }
    }

    if (temp_iter == fragments.end())
    {
        fragments.push_back(std::make_shared<Ipv4Fragment>(ipv4p));
        temp_iter = fragments.end() - 1;
    }

    auto frag = *temp_iter;
    frag->Add(ipv4p, is);

    if (frag->IsComplete())
    {
        //将完整的ip包数据提取出来
        std::stringstream ss;
        frag->get_data(ss);
        if (frag->get_protocol() == 88)  // eigrp
        {
            auto eigrpp = std::make_shared<EigrpPacket>(ss, frag->get_tlen());
            InsertL4Packet(eigrpp, frag);
        }
        else
        {
            std::cout << "unknow protocol: " << frag->get_protocol() << '\n';
        }
        //将用完的fragment删掉，节省空间和时间
        fragments.erase(temp_iter);
    }
}

void PcapFile::InsertL4Packet(std::shared_ptr<Packet> l4p, std::shared_ptr<Ipv4Fragment> frag)
{
    for (uint32_t i = 0; i < frag->get_ipv4p_num(); ++i)
    {
        for (size_t j = 0; j < packets.size(); ++j)
        {
            if (frag->get_ipv4p(i) == packets[j][2])
            {
                packets[j].push_back(l4p);
            }
        }
    }
}
