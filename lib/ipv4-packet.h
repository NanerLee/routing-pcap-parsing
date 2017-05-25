/**
* @Author: nanerlee
* @Date:   2017-01-17T15:14:36+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T19:37:22+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef IPV4_PACKET_H
#define IPV4_PACKET_H

#include <memory>

#include "address-conversion.h"
#include "packet.h"

struct Ipv4Header
{
    uint8_t  hlen : 4;   //头部长度
    uint8_t  ver : 4;    // IP协议版本
    uint8_t  tos;        //服务类型
    uint16_t tlen;       //总长度
    uint16_t id;         //标识符
    uint16_t fragment;   //分片标志和分片偏移
    uint8_t  ttl;        // ttl
    uint8_t  protocol;   //封装协议类型
    uint16_t checksum;   //校验和
    uint8_t  src_ip[4];  //源IP地址
    uint8_t  dst_ip[4];  //目的IP地址
    // other options
};

class Ipv4Packet : public Packet
{
  public:
    Ipv4Packet(std::istream& is);
    virtual ~Ipv4Packet() = default;
    void Print(std::ostream& os) override final;
    bool     IsFragment();
    uint32_t get_sip();
    uint32_t get_dip();
    uint16_t get_id();
    uint16_t get_flags();
    uint16_t get_offset();
    uint16_t get_tlen();
    uint8_t  get_protocol();

  private:
    Ipv4Header header;
    uint32_t   sip;
    uint32_t   dip;
    uint16_t   id;
    uint16_t   flags;
    uint16_t   offset;
    uint16_t   tlen;  //总长度
    uint8_t    protocol;
};

//------------------------------------------------------------------------------

class Ipv4Fragment
{
  public:
    Ipv4Fragment(std::shared_ptr<Ipv4Packet> p);
    virtual ~Ipv4Fragment() = default;
    void Add(std::shared_ptr<Ipv4Packet> p, std::istream& is);
    bool IsSameIpPacket(std::shared_ptr<Ipv4Packet> p);
    bool     IsComplete();
    uint8_t  get_protocol();
    uint16_t get_tlen();
    void get_data(std::stringstream& ss);
    uint32_t                    get_ipv4p_num();
    std::shared_ptr<Ipv4Packet> get_ipv4p(uint32_t index);

  private:
    std::vector<std::shared_ptr<Ipv4Packet>> ipv4_packets;

    uint32_t sip;
    uint32_t dip;
    uint16_t id;
    uint8_t  protocol;
    uint16_t tlen;            // ip数据的完整长度
    uint16_t clen;            //已写入的ip数据长度
    uint8_t  ip_data[65536];  //大小待定
};

#endif /* end of include guard: IPV4_PACKET_H */
