//
// Created by hadriax on 23/11/23.
//

#ifndef NEWCLIONTEST_IPADDRESS_H
#define NEWCLIONTEST_IPADDRESS_H

#include <cstdint>
#include <string>

union IP {
    uint32_t ip32;
    uint8_t ip8[4];
};

class IpAddress {
private:
    IP ip{};
    uint8_t cidr{};
    IP netmask{};
    IP network{};
    IP broadcast{};

public:
    explicit IpAddress(const std::string& ip_cidr);

private:
    void parse_ip(const std::string& ip_cidr);
public:
    static uint32_t create_netmask(uint8_t cidr);
    void display_ip() const;
    void display_cidr() const;
    static void display_ip(const IP& _ip);
    void display_binary_mask() const;
    void display_hexadecimal_mask() const;
    [[nodiscard]] IP get_network_address() const;
    [[nodiscard]] IP get_broadcast_address() const;

private:
    [[nodiscard]] IP create_network_address() const;
    [[nodiscard]] IP create_broadcast_address() const;
public:
    [[nodiscard]] uint32_t max_host_number() const;
};

#endif //NEWCLIONTEST_IPADDRESS_H
