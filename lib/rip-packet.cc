/**
* @Author: nanerlee
* @Date:   2017-02-14T16:02:57+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T20:34:07+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "rip-packet.h"

//--------------------------------RipTableEntry---------------------------------

RipTableEntry::RipTableEntry(std::istream& is)
{
    is.read((char*)&re, sizeof(RipEntry));
    prefix  = arr2num(re.prefix, 4);
    mask    = arr2num(re.mask, 4);
    nexthop = arr2num(re.nexthop, 4);
    metric  = ntohl(re.metric);
}

void RipTableEntry::Print(std::ostream& os)
{
    os << '\t' << "RipTableEntry:" << '\n';
    os << "\t\t"
       << "ip address: " << num2ip(prefix) << '\n';
    os << "\t\t"
       << "netmask: " << num2ip(mask) << '\n';
    os << "\t\t"
       << "nexthop: " << num2ip(nexthop) << '\n';
    os << "\t\t"
       << "metric: " << metric << '\n';
}

uint32_t RipTableEntry::get_prefix()
{
    return prefix;
}
uint32_t RipTableEntry::get_mask()
{
    return mask;
}
uint32_t RipTableEntry::get_nexthop()
{
    return nexthop;
}
uint32_t RipTableEntry::get_metric()
{
    return metric;
}

//------------------------------RipPacket---------------------------------------

RipPacket::RipPacket(std::istream& is, uint16_t len)
{
    is.read((char*)&header, sizeof(RipHeader));
    command = header.command;
    tlen    = len;
    len     = len - sizeof(RipHeader);
    while (len != 0)
    {
        auto temp_rte = std::make_shared<RipTableEntry>(is);
        rtes.push_back(temp_rte);
        len = len - sizeof(RipEntry);
    }
}

void RipPacket::Print(std::ostream& os)
{
    os << "Rip Packet:" << '\n';
    os << '\t' << "command: " << (uint32_t)command << '\n';  // uint8_t有些值是不可见字符
    os << '\t' << "total length: " << tlen << '\n';
    for (auto& var : rtes)
    {
        var->Print(os);
    }
    os << std::endl;
}

uint8_t RipPacket::get_command()
{
    return command;
}
uint16_t RipPacket::get_tlen()
{
    return tlen;
}
uint32_t RipPacket::get_rte_size()
{
    return rtes.size();
}
std::shared_ptr<RipTableEntry> RipPacket::get_rte(uint32_t index)
{
    return rtes[index];
}
