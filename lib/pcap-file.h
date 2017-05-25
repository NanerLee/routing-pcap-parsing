/**
* @Author: nanerlee
* @Date:   2017-02-20T15:54:20+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T21:15:52+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef PCAP_FILE_H
#define PCAP_FILE_H

#include <fstream>
#include <memory>

#include "eigrp-packet.h"
#include "ethernet-packet.h"
#include "ipv4-packet.h"
#include "ospf-packet.h"
#include "pcap-packet.h"
#include "ppp-packet.h"
#include "rip-packet.h"
#include "udp-packet.h"

struct PcapFileHeader
{
    uint32_t magic;  //字节顺序 0xa1b2c3d4为顺序，0xd4c3b2a1为逆序
    uint16_t version_major;  //主要版本号
    uint16_t version_minor;  //次要版本号
    uint32_t thiszone;       //时区
    uint32_t sigfigs;        //时间戳精度
    uint32_t snaplen;  //数据包的最大捕获长度，捕获每个数据包的前多少字节
    uint32_t linktype;  //链路类型，1为Ethernet，9为PPP，
};

class PcapFile
{
  public:
    PcapFile(std::string file);
    virtual ~PcapFile() = default;
    void     Parse();
    uint32_t get_count();
    // i：pcap包的序号。
    // j：0-pcap包头；1-二层包头(ppp和ethernet)；2-ip包头；
    // 3-四层包头(tcp和udp)或路由协议包(ospf和eigrp)；4-rip包和其他协议包。
    std::vector<std::shared_ptr<Packet>>& get_packet(uint32_t i);
    std::shared_ptr<Packet> get_packet(uint32_t i, uint32_t j);

  private:
    void HandleFragment(std::shared_ptr<Ipv4Packet> ipv4p, std::istream& is);
    void InsertL4Packet(std::shared_ptr<Packet> l4p, std::shared_ptr<Ipv4Fragment> frag);

    std::string    filename;
    PcapFileHeader fh;
    uint32_t       linktype;
    uint32_t       count;

    std::vector<std::vector<std::shared_ptr<Packet>>> packets;
    std::vector<std::shared_ptr<Ipv4Fragment>>        fragments;
};

#endif /* end of include guard: PCAP_FILE_H */
