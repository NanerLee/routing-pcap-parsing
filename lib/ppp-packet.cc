/**
* @Author: nanerlee
* @Date:   2017-01-16T15:09:31+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T09:42:25+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "ppp-packet.h"

PppPacket::PppPacket(std::istream& is)
{
    is.read((char*)&header, sizeof(PppHeader));
    protocol = ntohs(header.protocol);
};

void PppPacket::Print(std::ostream& os)
{
    os << "PPP Packet:" << '\n';
    os << '\t' << "protocol: "
       << "0x" << std::setfill('0') << std::setw(4) << std::hex << protocol << '\n';
    os << std::dec << std::endl;
}

uint16_t PppPacket::get_protocol()
{
    return protocol;
}
