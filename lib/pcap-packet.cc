/**
* @Author: nanerlee
* @Date:   2017-01-16T15:08:52+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-04-12T18:58:10+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "./pcap-packet.h"

PcapPacket::PcapPacket(std::istream& is)
{
    is.read((char*)&header, sizeof(PcapPacketHeader));
    time   = header.sec + (double)header.usec / 1000000;
    caplen = header.caplen;
    len    = header.len;
}

void PcapPacket::Print(std::ostream& os)
{
    os << "Pcap Packet:" << '\n';
    os << '\t' << "time: " << time << '\n';
    os << '\t' << "caplen: " << caplen << '\n';
    os << '\t' << "len: " << len << '\n';
    os << std::endl;
}

double PcapPacket::get_time()
{
    return time;
}
uint32_t PcapPacket::get_caplen()
{
    return caplen;
}
uint32_t PcapPacket::get_len()
{
    return len;
}
