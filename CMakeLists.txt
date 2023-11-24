cmake_minimum_required(VERSION 3.26)
project(IPv4_calculator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Ipv4_calculator main.cpp
        IpAddress.cpp
        IpAddress.h)
