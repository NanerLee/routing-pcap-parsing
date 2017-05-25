/**
* @Author: nanerlee
* @Date:   2017-02-18T09:47:35+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-25T14:36:55+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "address-conversion.h"

uint64_t arr2num(uint8_t* ch, uint8_t size)
{
    uint64_t result = 0;
    --size;
    for (size_t i = 0; i < size; ++i)
    {
        result = (result + *(ch + i)) * 0x100;
    }
    return result + *(ch + size);
}

uint64_t str2num(std::string str)
{
    size_t   pos    = 0;
    uint64_t result = 0;

    while (1)
    {
        result = result * 0x100 + std::stoi(str, &pos, 16);
        if (++pos == str.size())
        {
            break;
        }
        str = std::string(str.begin() + pos, str.end());
    }
    return result;
}

//-----------------------ip地址---------------------------

std::string num2ip(uint32_t i)
{
    std::vector<std::string> nums;
    for (size_t j = 0; j < 3; ++j)
    {
        nums.push_back(std::to_string(i % 0x100));
        i = (i - i % 0x100) / 0x100;
    }
    nums.push_back(std::to_string(i));

    return nums[3] + "." + nums[2] + "." + nums[1] + "." + nums[0];
}

//-------------------------------mac地址-----------------------------

std::string num2mac(uint64_t i)
{
    std::vector<uint32_t> nums;
    for (size_t j = 0; j < 5; ++j)
    {
        nums.push_back(i % 0x100);
        i = (i - i % 0x100) / 0x100;
    }
    nums.push_back(i);

    std::stringstream ss;
    ss << std::hex;
    for (size_t i = 5; i > 0; --i)
    {
        ss << std::setfill('0') << std::setw(2) << nums[i];
        ss << ":";
    }
    ss << std::setfill('0') << std::setw(2) << nums[0];
    return ss.str();
}

//-------------------------------网络掩码--------------------------------------

uint32_t len2num(uint32_t len)
{
    return 0xFFFFFFFF << (32 - len);
}

uint32_t num2len(uint32_t i)
{
    uint32_t len = 0;
    while (i != 0)
    {
        i = i << 1;
        ++len;
    }
    return len;
}
