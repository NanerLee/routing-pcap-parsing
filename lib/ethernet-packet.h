/**
* @Author: nanerlee
* @Date:   2017-01-16T14:53:33+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T16:32:45+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef ETHERNET_PACKET_H
#define ETHERNET_PACKET_H

#include "address-conversion.h"
#include "packet.h"

struct EthHeader
{
    uint8_t  dst_mac[6];  //目的MAC地址
    uint8_t  src_mac[6];  //源MAC地址
    uint16_t type;        /* packet type ID field */
};

class EthPacket : public Packet
{
  public:
    EthPacket(std::istream& is);
    virtual ~EthPacket() = default;
    void Print(std::ostream& os) override final;
    uint64_t get_smac();
    uint64_t get_dmac();
    uint16_t get_type();

  private:
    EthHeader header;
    uint64_t  smac;
    uint64_t  dmac;
    uint16_t  type;
};

#endif /* end of include guard: ETHERNET_PACKET_H */
