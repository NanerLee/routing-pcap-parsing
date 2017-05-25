/**
* @Author: nanerlee
* @Date:   2017-02-18T15:10:03+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T15:40:29+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef OSPF_PACKET_H
#define OSPF_PACKET_H

#include <memory>

#include "address-conversion.h"
#include "packet.h"

//------------------------------------------------------------------------------

class OspfContent
{
  public:
    OspfContent()                        = default;
    virtual ~OspfContent()               = default;
    virtual void Print(std::ostream& os) = 0;
    virtual bool IsAvailable()           = 0;
};

//------------------------------------------------------------------------------

struct HelloHeader
{
    uint8_t  mask[4];
    uint16_t hello_interval;
    uint8_t  options;
    uint8_t  priority;
    uint32_t dead_interval;
    uint8_t  d_router[4];
    uint8_t  b_router[4];
};

class HelloContent : public OspfContent
{
  public:
    HelloContent(std::istream& is, uint16_t len);
    virtual ~HelloContent() = default;
    void Print(std::ostream& os) override final;
    bool     IsAvailable() override final;
    uint32_t get_mask();
    uint32_t get_dr();
    uint32_t get_br();
    uint32_t get_nr_num();
    uint32_t get_nr(uint32_t index);

  private:
    HelloHeader           header;
    uint32_t              mask;
    uint32_t              dr;
    uint32_t              br;
    uint32_t              nr_num;
    std::vector<uint32_t> neighbors;
};

//------------------------------------------------------------------------------
struct LsaHeader
{
    uint16_t age;
    uint8_t  option;
    uint8_t  lsa_type;
    uint8_t  state_id[4];
    uint8_t  adv_router[4];
    uint32_t seq;
    uint16_t checksum;
    uint16_t tlen;
};

class Lsa
{
  public:
    Lsa(std::istream& is);
    virtual ~Lsa()                       = default;
    virtual void Print(std::ostream& os) = 0;
    uint8_t  get_type();
    uint32_t get_state_id();
    uint32_t get_adv_router();
    uint16_t get_tlen();

  protected:
    LsaHeader header;
    uint8_t   lsa_type;
    uint32_t  state_id;
    uint32_t  adv_router;
    uint16_t  tlen;
};

//------------------------------------------------------------------------------

class RouterLsaContent
{
  public:
    RouterLsaContent(std::istream& is);
    virtual ~RouterLsaContent() = default;
    void Print(std::ostream& os);
    uint32_t              get_link_id();
    uint32_t              get_link_data();
    uint8_t               get_link_type();
    uint8_t               get_tos_num();
    uint16_t              get_tos0_metric();
    std::vector<uint16_t> get_tos(uint8_t index);

  private:
    uint32_t                           link_id;
    uint32_t                           link_data;  //在p2p下是整型
    uint8_t                            link_type;
    uint8_t                            tos_num;
    uint16_t                           tos0_metric;
    std::vector<std::vector<uint16_t>> toses;
};

class RouterLsa : public Lsa
{
  public:
    RouterLsa(std::istream& is);
    virtual ~RouterLsa() = default;
    void Print(std::ostream& os) override final;
    uint16_t                          get_num();
    std::shared_ptr<RouterLsaContent> get_content(uint16_t index);

  private:
    uint16_t                                       options;
    uint16_t                                       link_num;
    std::vector<std::shared_ptr<RouterLsaContent>> contents;
};

//------------------------------------------------------------------------------

class NetworkLsa : public Lsa
{
  public:
    NetworkLsa(std::istream& is);
    virtual ~NetworkLsa() = default;
    void Print(std::ostream& os) override final;
    uint32_t get_mask();
    uint32_t get_attach_num();
    uint32_t get_attach_router(uint32_t index);

  private:
    uint32_t              mask;
    uint32_t              attach_num;
    std::vector<uint32_t> attach_routers;
};

//------------------------------------------------------------------------------
//其他三种lsa
//------------------------------------------------------------------------------

class LsuContent : public OspfContent
{
  public:
    LsuContent(std::istream& is);
    virtual ~LsuContent() = default;
    void Print(std::ostream& os) override final;
    bool                 IsAvailable() override final;  //判断数据包是否可用，即是否有1,2类型的lsa（其它的lsa被略过）
    uint32_t             get_lsa_num();
    std::shared_ptr<Lsa> get_lsa(uint32_t index);

  private:
    uint32_t                          lsa_num;
    std::vector<std::shared_ptr<Lsa>> lsas;
};

//------------------------------------------------------------------------------

struct OspfHeader
{
    uint8_t  version;
    uint8_t  msg_type;
    uint16_t tlen;
    uint8_t  src_router[4];
    uint8_t  area[4];
    uint16_t checksum;
    uint16_t auth_type;
    uint64_t auth_data;
};

class OspfPacket : public Packet
{
  public:
    OspfPacket(std::istream& is, uint32_t len);
    virtual ~OspfPacket() = default;
    void Print(std::ostream& os) override final;
    bool                         IsComplete();
    uint8_t                      get_type();
    uint16_t                     get_tlen();
    uint32_t                     get_src_router();
    uint32_t                     get_area();
    std::shared_ptr<OspfContent> get_content();

  private:
    OspfHeader                   header;
    uint8_t                      type;
    uint16_t                     tlen;
    uint32_t                     src_router;
    uint32_t                     area;
    std::shared_ptr<OspfContent> content;
};

#endif /* end of include guard: OSPF_PACKET_H */
