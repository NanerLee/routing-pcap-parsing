/**
* @Author: nanerlee
* @Date:   2017-01-16T10:17:55+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T16:26:11+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <vector>

//抽象基类，仅用于继承
class Packet
{
  public:
    Packet()                             = default;
    virtual ~Packet()                    = default;
    virtual void Print(std::ostream& os) = 0;
};

#endif /* end of include guard: PACKET_H */
