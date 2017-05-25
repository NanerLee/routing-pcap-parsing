/**
* @Author: nanerlee
* @Date:   2017-01-16T15:12:38+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-04-12T18:47:08+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef UDP_PACKET_H
#define UDP_PACKET_H

#include "packet.h"

struct UdpHeader
{
    uint16_t src_port;  //源端口
    uint16_t dst_port;  //目的端口
    uint16_t len;       //长度
    uint16_t checksum;  //校验和
};

class UdpPacket : public Packet
{
  public:
    UdpPacket(std::istream& is);
    virtual ~UdpPacket() = default;
    void Print(std::ostream& os) override final;
    uint16_t get_sport();
    uint16_t get_dport();
    uint16_t get_tlen();

  private:
    UdpHeader header;
    uint16_t  sport;
    uint16_t  dport;
    uint16_t  tlen;
};

#endif /* end of include guard: UDP_PACKET_H */
