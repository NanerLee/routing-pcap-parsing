/**
* @Author: nanerlee
* @Date:   2017-02-20T16:33:56+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T16:55:49+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "eigrp-packet.h"

//------------------------------------Tlv---------------------------------------

Tlv::Tlv(std::istream& is)
{
    is.read((char*)&type, 2);
    type = ntohs(type);
    is.read((char*)&tlen, 2);
    tlen = ntohs(tlen);
}

uint16_t Tlv::get_type()
{
    return type;
}
uint16_t Tlv::get_tlen()
{
    return tlen;
}

//---------------------------derived Tlv----------------------------------------
// ParameterTlv
ParameterTlv::ParameterTlv(std::istream& is) : Tlv(is)
{
    is.read((char*)&k, 6);
    is.read((char*)&hold_time, 2);
    hold_time = ntohs(hold_time);
}

void ParameterTlv::Print(std::ostream& os)
{
    os << '\t' << "Parameters:" << '\n';
    os << "\t\t"
       << "type: "
       << "0x0001" << '\n';
    os << "\t\t"
       << "length: " << tlen << '\n';
    os << "\t\t"
       << "k1: " << (uint16_t)k[0] << '\n';
    os << "\t\t"
       << "k2: " << (uint16_t)k[1] << '\n';
    os << "\t\t"
       << "k4: " << (uint16_t)k[2] << '\n';
    os << "\t\t"
       << "k3: " << (uint16_t)k[3] << '\n';
    os << "\t\t"
       << "k5: " << (uint16_t)k[4] << '\n';
    os << "\t\t"
       << "k6: " << (uint16_t)k[5] << '\n';
    os << "\t\t"
       << "hold time: " << hold_time << '\n';
}

uint8_t ParameterTlv::get_kn(uint32_t n)
{
    return k[n - 1];
}
uint16_t ParameterTlv::get_hold_time()
{
    return hold_time;
}

// SoftwareVersionTlv
SoftwareVersionTlv::SoftwareVersionTlv(std::istream& is) : Tlv(is)
{
    uint8_t arr[4];
    is.read((char*)&arr, 4);
    release_version = std::to_string(arr[0]) + "." + std::to_string(arr[1]);
    tlv_version     = std::to_string(arr[2]) + "." + std::to_string(arr[3]);
}

void SoftwareVersionTlv::Print(std::ostream& os)
{
    os << '\t' << "Software Version:" << '\n';
    os << "\t\t"
       << "type: "
       << "0x0004" << '\n';
    os << "\t\t"
       << "length: " << tlen << '\n';
    os << "\t\t"
       << "eigrp release: " << release_version << '\n';
    os << "\t\t"
       << "tlv version: " << tlv_version << '\n';
}

std::string SoftwareVersionTlv::get_release_version()
{
    return release_version;
}
std::string SoftwareVersionTlv::get_tlv_version()
{
    return tlv_version;
}

// Ipv4InternalTlv
Ipv4InternalTlv::Ipv4InternalTlv(std::istream& is) : Tlv(is)
{
    uint8_t arr[4];
    is.read((char*)&arr, 4);
    nexthop = arr2num(arr, 4);

    is.read((char*)&metric, sizeof(Metric));
    delay     = ntohl(metric.delay);
    bandwidth = ntohl(metric.bandwidth);
    mtu       = ntohl(metric.mtu_hop_count) >> 8;
    hop_count = ntohl(metric.mtu_hop_count) & 0xFF;

    is.read((char*)&prefixlen, 1);
    uint8_t arr1[4] = {0, 0, 0, 0};
    //子网掩码的大小影响目的地址的长度
    uint16_t dst_len =
        tlen - 4 - 4 - sizeof(metric) - 1;  //或(prefixlen - 1) / 8 + 1;
    is.read((char*)&arr1, dst_len);
    destination = arr2num(arr1, 4);
}

void Ipv4InternalTlv::Print(std::ostream& os)
{
    os << '\t' << "Ipv4 Internal TLV:" << '\n';
    os << "\t\t"
       << "type: "
       << "0x0102" << '\n';
    os << "\t\t"
       << "length: " << tlen << '\n';
    os << "\t\t"
       << "nexthop: " << num2ip(nexthop) << '\n';
    os << "\t\t"
       << "prefix length: " << (uint32_t)prefixlen << '\n';
    os << "\t\t"
       << "destination: " << num2ip(destination) << '\n';
    os << "\t\t"
       << "metric:" << '\n';
    os << "\t\t\t"
       << "scaled delay: " << delay << '\n';
    os << "\t\t\t"
       << "scaled bandwidth: " << bandwidth << '\n';
    os << "\t\t\t"
       << "mtu: " << mtu << '\n';
    os << "\t\t\t"
       << "hop count: " << hop_count << '\n';
}

uint32_t Ipv4InternalTlv::get_nexthop()
{
    return nexthop;
}
uint32_t Ipv4InternalTlv::get_delay()
{
    return delay;
}
uint32_t Ipv4InternalTlv::get_bandwidth()
{
    return bandwidth;
}
uint32_t Ipv4InternalTlv::get_mtu()
{
    return mtu;
}
uint32_t Ipv4InternalTlv::get_hop_count()
{
    return hop_count;
}
uint8_t Ipv4InternalTlv::get_prefixlen()
{
    return prefixlen;
}
uint32_t Ipv4InternalTlv::get_destination()
{
    return destination;
}
//------------------------------EigrpPacket-------------------------------------

EigrpPacket::EigrpPacket(std::istream& is, uint16_t len)
{
    is.read((char*)&header, sizeof(EigrpHeader));
    opcode   = header.opcode;
    flags    = ntohl(header.flags);
    v_router = ntohs(header.v_router);
    as       = ntohs(header.as);

    len = len - sizeof(EigrpHeader);  //剩余的数据长度

    if (opcode == 1)  // update
    {
        while (len != 0)
        {
            auto     pos = is.tellg();
            uint16_t temp[2];  // temp tlv header
            is.read((char*)&temp, 4);
            temp[0] = ntohs(temp[0]);  // tlv type
            temp[1] = ntohs(temp[1]);  // tlv len
            is.seekg(pos);
            if (temp[0] == 0x0102)
            {
                auto internal_tlv = std::make_shared<Ipv4InternalTlv>(is);
                tlvs.push_back(internal_tlv);
            }
            else  //跳过其他类型tlv： external tlv 和 community tlv
            {
                // std::cout << "ignore other tlv -- type: " << temp[0] << '\n';
                is.seekg(temp[1], std::ios::cur);
            }
            len = len - temp[1];
        }
    }
    else if (opcode == 5 && len != 0)  // hello
    {
        auto parameter = std::make_shared<ParameterTlv>(is);
        tlvs.push_back(parameter);
        auto software_version = std::make_shared<SoftwareVersionTlv>(is);
        tlvs.push_back(software_version);

        len = len - parameter->get_tlen() - software_version->get_tlen();
        is.seekg(len, std::ios::cur);
    }
    else  //跳过其他类型的包
    {
        // std::cout << "ignore other packet -- opcode: " << (uint16_t)opcode <<
        // '\n';
        is.seekg(len, std::ios::cur);
    }
}

void EigrpPacket::Print(std::ostream& os)
{
    os << "Eigrp Packet:" << '\n';
    os << '\t' << "opcode: " << (uint16_t)opcode << '\n';
    os << '\t' << "flags: " << flags << '\n';
    os << '\t' << "virtual router id: " << v_router << '\n';
    os << '\t' << "autonomous system: " << as << '\n';
    os << std::endl;

    for (auto& var : tlvs)
    {
        var->Print(os);
    }
    os << std::endl;
}

uint8_t EigrpPacket::get_opcode()
{
    return opcode;
}
uint32_t EigrpPacket::get_flags()
{
    return flags;
}
uint16_t EigrpPacket::get_v_router()
{
    return v_router;
}
uint16_t EigrpPacket::get_as()
{
    return as;
}
uint32_t EigrpPacket::get_tlv_num()
{
    return tlvs.size();
}
std::shared_ptr<Tlv> EigrpPacket::get_tlv(uint32_t index)
{
    return tlvs[index];
}
