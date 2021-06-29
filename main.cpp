#include <iostream>

#include "crc64.hpp"



int main() {
    auto x = crc64::Digest {};
    x.write("hello, world!", 12);
    std::cout << x.checksum() << std::endl;
    return 0;
}
