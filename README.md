# routing-pcap-parsing

Parse pcap files for routing protocol(RIP, OSPF, EIGRP)

主要实现一个路由报文中信息的解析库, 支持RIP, OSPF, EIGRP协议.

## 支持协议

| 协议 | 支持情况 |
|:-------:|:-------:|
| 802.3 | 完全支持 |
| PPP | 完全支持 |
| IPv4 | 完全支持 |
| UDP | 完全支持 |
| RIPv2 | 完全支持 |
| OSPFv2 | 部分支持 |
| EIGRP | 部分支持 |

- 对于 EIGRP 协议, 目前能够解析的报文有:
    - Hello 报文
    - Update 报文
    - 其中支持的 TLV 有:
        - Parameter TLV
        - SoftwareVersion TLV
        - Ipv4 Internal TLV
- 对于 OSPFv2 协议, 目前能够解析的报文包括:
    - Hello Packet
    - LSU 报文, 其中支持的 LSA 有:
        - 路由器链路 LSA
        - 网络链路 LSA

## 解析结果的数据结构

pcap 文件解析完成之后会称为一个 `PcapFile` 类, 各层协议包以数组形式存储, 形成一个二维数组如下:

| 序号 | 0 | 1 | 2 | 3 | 4 |
|:---:|:-------:|:-------:|:-------:|:-------:|:-------:|
| 0 | pcap-packet | l2-packet | ipv4-packet | udp-packet | rip-packet |
| 1 | pcap-packet | l2-packet | ipv4-packet | ospf-packet |
| 2 | pcap-packet | l2-packet | ipv4-packet | eigrp-packet |


## 用法

1. clone 项目
2. 在 src 文件夹中添加自己的源代码
3. 将头文件 `pcap-file.h` 引入到自己的文件中
4. 在 src 文件夹下的 `CMakeLists.txt` 中添加自己程序的生成项, 并链接库文件
5. 在项目根目录执行以下命令进行编译:
    ```shell
    cd build
    cmake ..  # 需要系统安装 cmake
    make
    ```
6. 生成的可执行文件在 bin 目录下, 运行即可

## 使用范例

```shell
cd build
cmake ..
make
cd ..

# 测试
./bin/print ./test/rip-test1.pcap
./bin/print ./test/ospf-test2.cap
./bin/print ./test/eigep-test1.pcap
```

## 注意

- 默认支持pcap文件, cap可能会出现解析错误
- 对于不支持的协议, 解析库将会跳过相应的数据
- 对于 IP 分片数据, 如果重组成功后, 对应的每一个 IP 报文后面都是一个完整的报文
- 一个报文包, 除了最高层次的那个协议包是完整数据外, 其余的都是只包含协议包头的数据.
- 由于时间和精力问题, 对协议的支持不足, 项目的使用也不人性化, 欢迎建议和贡献.
