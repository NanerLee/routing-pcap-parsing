/**
* @Author: nanerlee
* @Date:   2017-01-16T14:53:36+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T16:32:57+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "ethernet-packet.h"

EthPacket::EthPacket(std::istream& is)
{
    is.read((char*)&header, sizeof(EthHeader));
    smac = arr2num(header.src_mac, 6);
    dmac = arr2num(header.dst_mac, 6);
    type = ntohs(header.type);
}

void EthPacket::Print(std::ostream& os)
{
    os << "Ethernet Packet:" << '\n';
    os << '\t' << "source mac: " << num2mac(smac) << '\n';
    os << '\t' << "destination mac: " << num2mac(dmac) << '\n';
    os << '\t' << "type: "
       << "0x" << std::setfill('0') << std::setw(4) << std::hex << type << '\n';
    os << std::dec << std::endl;  // 恢复输出的进制
}

uint64_t EthPacket::get_smac()
{
    return smac;
}
uint64_t EthPacket::get_dmac()
{
    return dmac;
}
uint16_t EthPacket::get_type()
{
    return type;
}
