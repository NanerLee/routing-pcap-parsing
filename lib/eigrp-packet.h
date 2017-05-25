/**
* @Author: nanerlee
* @Date:   2017-02-16T14:53:20+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T20:09:17+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef EIGPR_PACKET_H
#define EIGPR_PACKET_H

#include <memory>

#include "address-conversion.h"
#include "packet.h"

//------------------------------------------------------------------------------

class Tlv
{
  public:
    Tlv(std::istream& is);
    virtual ~Tlv()                       = default;
    virtual void Print(std::ostream& os) = 0;
    uint16_t get_type();
    uint16_t get_tlen();

  protected:
    uint16_t type;
    uint16_t tlen;
};

//------------------------------------------------------------------------------

class ParameterTlv : public Tlv
{
  public:
    ParameterTlv(std::istream& is);
    virtual ~ParameterTlv() = default;
    void Print(std::ostream& os) override final;
    uint8_t get_kn(uint32_t n);  // n代表不是下标，而是kn
    uint16_t get_hold_time();

  private:
    uint8_t  k[6];
    uint16_t hold_time;
};

class SoftwareVersionTlv : public Tlv
{
  public:
    SoftwareVersionTlv(std::istream& is);
    virtual ~SoftwareVersionTlv() = default;
    void Print(std::ostream& os) override final;
    std::string get_release_version();
    std::string get_tlv_version();

  private:
    std::string release_version;
    std::string tlv_version;
};

struct Metric
{
    uint32_t delay;
    uint32_t bandwidth;
    uint32_t mtu_hop_count;
    uint8_t  reliability;
    uint8_t  load;
    uint8_t  router_tag;
    uint8_t  flags;
};

class Ipv4InternalTlv : public Tlv
{
  public:
    Ipv4InternalTlv(std::istream& is);
    virtual ~Ipv4InternalTlv() = default;
    void Print(std::ostream& os) override final;
    uint32_t get_nexthop();
    uint32_t get_delay();
    uint32_t get_bandwidth();
    uint32_t get_mtu();
    uint32_t get_hop_count();
    uint8_t  get_prefixlen();
    uint32_t get_destination();

  private:
    uint32_t nexthop;
    Metric   metric;
    uint32_t delay;
    uint32_t bandwidth;
    uint32_t mtu;
    uint32_t hop_count;
    uint8_t  prefixlen;
    uint32_t destination;
};

//------------------------------------------------------------------------------

struct EigrpHeader
{
    uint8_t  version;
    uint8_t  opcode;  // 1-update, 2-request, 3-query, 4-reply, 5-hello, etc.
    uint16_t checksum;
    uint32_t flags;  // 1-init, 2-cr, 3-rs, 4-eot
    uint32_t seq;
    uint32_t ack;
    uint16_t v_router;
    uint16_t as;
};

class EigrpPacket : public Packet
{
  public:
    EigrpPacket(std::istream& is, uint16_t len);
    virtual ~EigrpPacket() = default;
    void Print(std::ostream& os) override final;

    uint8_t  get_opcode();
    uint32_t get_flags();
    uint16_t get_v_router();
    uint16_t get_as();
    uint32_t get_tlv_num();

    std::shared_ptr<Tlv> get_tlv(uint32_t index);

  private:
    EigrpHeader header;
    uint8_t     opcode;
    uint32_t    flags;
    uint16_t    v_router;
    uint16_t    as;

    std::vector<std::shared_ptr<Tlv>> tlvs;
};

#endif /* end of include guard: EIGPR_PACKET_H */
