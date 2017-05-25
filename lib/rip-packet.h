/**
* @Author: nanerlee
* @Date:   2017-02-15T11:05:56+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T19:45:37+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef RIP_PACKET_H
#define RIP_PACKET_H

#include <memory>

#include "address-conversion.h"
#include "packet.h"

struct RipHeader
{
    uint8_t  command;
    uint8_t  version;
    uint16_t reserve;
};

struct RipEntry
{
    uint16_t family;
    uint16_t tag;
    uint8_t  prefix[4];
    uint8_t  mask[4];
    uint8_t  nexthop[4];
    uint32_t metric;
};

class RipTableEntry
{
  public:
    RipTableEntry(std::istream& is);
    virtual ~RipTableEntry() = default;
    void Print(std::ostream& os);
    uint32_t get_prefix();
    uint32_t get_mask();
    uint32_t get_nexthop();
    uint32_t get_metric();

  private:
    RipEntry re;
    uint32_t prefix;
    uint32_t mask;
    uint32_t nexthop;
    uint32_t metric;
};

//------------------------------------------------------------------------------

class RipPacket : public Packet
{
  public:
    RipPacket(std::istream& is, uint16_t len);
    virtual ~RipPacket() = default;
    void Print(std::ostream& os) override final;
    uint8_t                        get_command();
    uint16_t                       get_tlen();
    uint32_t                       get_rte_size();
    std::shared_ptr<RipTableEntry> get_rte(uint32_t index);

  private:
    RipHeader                                   header;
    uint8_t                                     command;
    uint16_t                                    tlen;
    std::vector<std::shared_ptr<RipTableEntry>> rtes;
};

#endif /* end of include guard: RIP_PACKET_H */
