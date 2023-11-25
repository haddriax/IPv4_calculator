//
// Created by hadriax on 23/11/23.
//

#include <charconv>
#include <iostream>
#include <sstream>
#include <limits>
#include <bitset>
#include "IpAddress.h"

IpAddress::IpAddress(const std::string &ip_cidr) {
    if (ip_cidr.size() > 18) {
        throw std::invalid_argument("Given IP does not match max IP size");
    }

    bool format_is_valid = true;

    int dot_counter = 0;
    bool have_slash = false;
    for (const auto c: ip_cidr) {
        if (c == '.')
            ++dot_counter;
        if (c == '/')
            have_slash = true;

        if (!std::isdigit(static_cast<unsigned char>(c)) &&
            c != '.' &&
            c != '/') {
            throw std::invalid_argument("Unexpected character in IP.19");
        }
    }
    format_is_valid = (dot_counter == 3) && have_slash;
    if (!format_is_valid) {
        throw std::invalid_argument("Given IP does not match format rules");
    }

    parse_ip(ip_cidr);
    netmask.ip32 = create_netmask(cidr);
    network = create_network_address();
    broadcast = create_broadcast_address();
}

void IpAddress::parse_ip(const std::string& ip_cidr) {
    // Declare an istringstream to process the string.
    std::istringstream tokenStream(ip_cidr);
    // Find the index where the CIDR begin, i.e. the '/' separator.
    const auto cidr_begin = ip_cidr.find_first_of('/');

    if (cidr_begin != std::string::npos) {
        // Get the CIDR from our string.
        int tmp = 0;
        std::string tmp_str = ip_cidr.substr((cidr_begin + 1), ip_cidr.size());

        // Convert literal CIDR to int
        {
            auto [p, ec] =
                    std::from_chars(tmp_str.data(), tmp_str.data() + tmp_str.size(), tmp);

        }

        if (tmp > 32) {
            throw std::invalid_argument("Given CIDR is > 32.");
        }

        // Convert and store CIDR.
        cidr = static_cast<uint8_t>(tmp);

        short i = 4;
        uint32_t v = 0;
        // Parse IP address using '.' separator.
        while (std::getline(tokenStream, tmp_str, '.')) {
            // Convert literal IP Bytes to int.
            auto [p, ec] =
                    std::from_chars(tmp_str.data(), tmp_str.data() + tmp_str.size(), v);

            if (v > 255)
                throw std::invalid_argument("One or more Bytes value is invalid.");

            // Store the integer representation of the Byte in the union as a 8bits unsigned int.
            ip.ip8[--i] = static_cast<uint8_t>(v);
        }
    }
}

uint32_t IpAddress::create_netmask(uint8_t cidr) {
    // Set all netmask bits to 1.
    uint32_t mask = std::numeric_limits<uint32_t>::max();
    // Binary shifting of the size of the mask multiplied by 8 to gets size in bits.
    // Then subtracting the cidr to get the number of bits to set to 0.
    mask <<= sizeof(mask) * 8 - cidr;
    return mask;
}

void IpAddress::display_ip() const {
    IpAddress::display_ip(this->ip);
}

void IpAddress::display_cidr() const {
    std::cout << static_cast<uint32_t>(cidr);
}

void IpAddress::display_netmask() const {
    for (auto i = 3; i >= 0; --i) {
        std::cout << std::dec << static_cast<uint32_t>(netmask.ip8[i]);
        if (i != 0) std::cout << '.';
    }
}

void IpAddress::display_binary_mask() const {
    std::cout << "Binary netmask: " << std::bitset<sizeof(netmask.ip32) * 8>{netmask.ip32} << std::endl;
}

void IpAddress::display_hexadecimal_mask() const {
    std::cout << std::hex <<  "Hexadecimal netmask: " << netmask.ip32 << std::dec << std::endl;
}

IP IpAddress::create_network_address() const {
    IP result{ ip.ip32 };
    result.ip32 = result.ip32 & netmask.ip32;
    return result;
}

void IpAddress::display_ip(const IP &_ip) {
    for (auto i = 3; i >= 0; --i) {
        std::cout << std::dec << static_cast<uint32_t>(_ip.ip8[i]);
        if (i != 0) std::cout << '.';
    }
}

IP IpAddress::create_broadcast_address() const {
    IP result{ ip.ip32 };
    result.ip32 = result.ip32 | (std::numeric_limits<uint32_t>::max() ^ netmask.ip32);
    return result;
}

uint32_t IpAddress::max_host_number() const {
    // int made from netmask where bits are 0. Minus broadcast and network address.
    return (std::numeric_limits<uint32_t>::max() ^ netmask.ip32) - 1;
}

IP IpAddress::get_network_address() const {
    return network;
}

IP IpAddress::get_broadcast_address() const {
    return broadcast;
}