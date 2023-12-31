#include <charconv>
#include <iostream>
#include <limits>
#include <bitset>

#include "IpAddress.h"

void test_ip(const std::string& input) {
    try {
        IpAddress ip(input);
        std::cout << "IP address is : ";
        ip.display_ip();
        std::cout << std::endl;

        std::cout << "CIDR is : ";
        ip.display_cidr();
        std::cout << std::endl;

        std::cout << "Netmask is : ";
        ip.display_netmask();
        std::cout << std::endl;

        std::cout << "Network address is : ";
        IpAddress::display_ip(ip.get_network_address());
        std::cout << std::endl;

        std::cout << "Broadcast is : ";
        IpAddress::display_ip(ip.get_broadcast_address());
        std::cout << std::endl;

        std::cout << "Max num of hosts : " << ip.max_host_number() << std::endl;
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << ' ' <<e.what() << std::endl;
    }
}

int main() {
    std::string input_ip;

    while (!((input_ip.starts_with('q') && input_ip.size() == 1)||
    input_ip == "quit" ||
    input_ip == "exit"))
    {
        std::cout << "Input IP with CIDR: ";
        std::cin >> input_ip;
        test_ip(input_ip);
    }
    // test_ip("192.168.6.15/24");
    // test_ip("214.133.67.154/8");
    // test_ip("174.54.11.88/19");
    // test_ip("174.54.11.88/30");
    return 0;
}
