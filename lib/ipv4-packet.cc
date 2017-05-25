/**
* @Author: nanerlee
* @Date:   2017-01-16T14:55:56+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T10:01:14+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "ipv4-packet.h"

//--------------------------------Ipv4Packet------------------------------------

Ipv4Packet::Ipv4Packet(std::istream& is)
{
    is.read((char*)&header, sizeof(Ipv4Header));
    sip      = arr2num(header.src_ip, 4);
    dip      = arr2num(header.dst_ip, 4);
    id       = ntohs(header.id);
    flags    = ntohs(header.fragment) >> 13;
    offset   = (ntohs(header.fragment) & 0x1FFF) * 8;  //以8字节为单位
    tlen     = ntohs(header.tlen);
    protocol = header.protocol;
}

void Ipv4Packet::Print(std::ostream& os)
{
    os << "Ipv4 Packet:" << '\n';
    os << '\t' << "source ip: " << num2ip(sip) << '\n';
    os << '\t' << "destination ip: " << num2ip(dip) << '\n';
    os << '\t' << "total length: " << tlen << '\n';
    os << '\t' << "identification: " << id << '\n';
    os << '\t' << "flags: " << flags << '\n';
    os << '\t' << "fragment offset: " << offset << '\n';
    os << '\t' << "protocol: " << (uint32_t)protocol << '\n';
    os << std::endl;
}

bool Ipv4Packet::IsFragment()
{
    if (offset > 0 || flags == 1)
    {
        return true;
    }
    return false;
}

uint32_t Ipv4Packet::get_sip()
{
    return sip;
}
uint32_t Ipv4Packet::get_dip()
{
    return dip;
}
uint16_t Ipv4Packet::get_id()
{
    return id;
}
uint16_t Ipv4Packet::get_flags()
{
    return flags;
}
uint16_t Ipv4Packet::get_offset()
{
    return offset;
}
uint16_t Ipv4Packet::get_tlen()
{
    return tlen;
}
uint8_t Ipv4Packet::get_protocol()
{
    return protocol;
}

//--------------------------------Ipv4Fragment----------------------------------

Ipv4Fragment::Ipv4Fragment(std::shared_ptr<Ipv4Packet> p)
    : sip(p->get_sip()), dip(p->get_dip()), id(p->get_id()),
      protocol(p->get_protocol()), tlen(0), clen(0)
{
}

void Ipv4Fragment::Add(std::shared_ptr<Ipv4Packet> p, std::istream& is)
{
    is.read((char*)&ip_data[p->get_offset()], p->get_tlen() - sizeof(Ipv4Header));
    if (p->get_flags() == 0)
    {
        tlen = p->get_offset() + p->get_tlen() - sizeof(Ipv4Header);
    }
    clen = clen + p->get_tlen() - sizeof(Ipv4Header);
    ipv4_packets.push_back(p);
}

bool Ipv4Fragment::IsSameIpPacket(std::shared_ptr<Ipv4Packet> p)
{
    if (p->get_sip() == sip && p->get_dip() == dip && p->get_id() == id &&
        p->get_protocol() == protocol)
    {
        return true;
    }
    return false;
}

bool Ipv4Fragment::IsComplete()
{
    if (tlen != 0 && tlen == clen)
    {
        return true;
    }
    return false;
}

uint8_t Ipv4Fragment::get_protocol()
{
    return protocol;
}
uint16_t Ipv4Fragment::get_tlen()
{
    return tlen;
}
void Ipv4Fragment::get_data(std::stringstream& ss)
{
    ss.write((char*)ip_data, tlen);
}
uint32_t Ipv4Fragment::get_ipv4p_num()
{
    return ipv4_packets.size();
}
std::shared_ptr<Ipv4Packet> Ipv4Fragment::get_ipv4p(uint32_t index)
{
    return ipv4_packets[index];
}
