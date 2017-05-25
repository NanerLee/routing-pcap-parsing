/**
* @Author: nanerlee
* @Date:   2017-02-18T09:47:30+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T20:09:05+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#ifndef ADDRESS_CONVERSION_H
#define ADDRESS_CONVERSION_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// 数组转数值
uint64_t arr2num(uint8_t* ch, uint8_t size);

// 转字符串转数值表示, 包括ip地址, mac地址
uint64_t strt2num(std::string str);

// 数值转ip
std::string num2ip(uint32_t i);

// 数值转mac
std::string num2mac(uint64_t i);

// 子网前缀长度转数值
uint32_t len2num(uint32_t len);

// 数值转前缀长度
uint32_t num2len(uint32_t i);

#endif /* end of include guard: ADDRESS_CONVERSION_H */
