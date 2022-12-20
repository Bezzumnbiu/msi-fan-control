#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
        return EXIT_FAILURE;


    std::fstream file;
    file.open("/sys/kernel/debug/ec/ec0/io", std::ios::binary | std::ios::out | std::ios::in);
    if (file.is_open()) {


        if (std::string(argv[1]) == "enable") {
            file.seekp(0x98);
            file.put(0x80);
            std::cout << "Enable CoolerBoost" << std::endl;
        }

        if (std::string(argv[1]) == "disable") {
            file.seekp(0x98);
            file.put(0x02);
            std::cout << "Disable CoolerBoost" << std::endl;
        }

        file.close();
    }
}
