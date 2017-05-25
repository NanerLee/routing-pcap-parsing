/**
* @Author: nanerlee
* @Date:   2017-01-16T15:08:49+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-04-12T18:46:33+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef PCAP_PACKET_H
#define PCAP_PACKET_H

#include "packet.h"

struct PcapPacketHeader
{
    uint32_t sec;     //时间戳高位，秒
    uint32_t usec;    //时间戳低位，微秒
    uint32_t caplen;  //捕获到的数据帧长度
    uint32_t len;     //原始数据帧长度
};

// pcap包
class PcapPacket : public Packet
{
  public:
    PcapPacket(std::istream& is);
    virtual ~PcapPacket() = default;
    void Print(std::ostream& os) override final;
    double   get_time();
    uint32_t get_caplen();
    uint32_t get_len();

  private:
    PcapPacketHeader header;
    double           time;
    uint32_t         caplen;
    uint32_t         len;
};

#endif /* end of include guard: PCAP_PACKET_H */
