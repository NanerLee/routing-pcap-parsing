/**
* @Author: nanerlee
* @Date:   2017-01-19T15:39:09+08:00
* @Email:  nanerlee@qq.com
 * @Last modified by:   nanerlee
 * @Last modified time: 2017-05-24T21:24:36+08:00
* @Copyright: Copyright (c) by NanerLee. All Rights Reserved.
*/

#include "pcap-file.h"

int main(int argc, const char* argv[])
{
    std::string filename = argv[1];

    PcapFile file(filename);
    file.Parse();

    for (size_t i = 0; i < file.get_count(); ++i)
    {
        auto p = file.get_packet(i);
        for (size_t j = 0; j < p.size(); ++j)
        {
            p[j]->Print(std::cout);
        }
    }

    return 0;
}
