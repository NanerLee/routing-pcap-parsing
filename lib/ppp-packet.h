/**
* @Author: nanerlee
* @Date:   2017-01-16T15:09:29+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-04-12T18:46:47+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef PPP_PACKET_H
#define PPP_PACKET_H

#include "address-conversion.h"
#include "packet.h"

struct PppHeader
{
    uint16_t protocol;
};

class PppPacket : public Packet
{
  public:
    PppPacket(std::istream& is);
    virtual ~PppPacket() = default;
    void Print(std::ostream& os) override final;
    uint16_t get_protocol();

  private:
    PppHeader header;
    uint16_t  protocol;
};

#endif /* end of include guard: PPP_PACKET_H */
