/**
* @Author: nanerlee
* @Date:   2017-01-16T15:12:18+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-04-12T18:58:58+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "udp-packet.h"

UdpPacket::UdpPacket(std::istream& is)
{
    is.read((char*)&header, sizeof(UdpHeader));
    sport = ntohs(header.src_port);
    dport = ntohs(header.dst_port);
    tlen  = ntohs(header.len);
};

void UdpPacket::Print(std::ostream& os)
{
    os << "UdpPacket:" << '\n';
    os << '\t' << "source port: " << sport << '\n';
    os << '\t' << "destination port: " << dport << '\n';
    os << '\t' << "total length: " << tlen << '\n';
    os << std::endl;
}

uint16_t UdpPacket::get_sport()
{
    return sport;
}
uint16_t UdpPacket::get_dport()
{
    return dport;
}
uint16_t UdpPacket::get_tlen()
{
    return tlen;
}
