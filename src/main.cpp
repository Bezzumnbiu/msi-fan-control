#include "defines.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

// For futures //

//int readACBConfig() {
//
//    ifstream inFile;
//    inFile.open(AUTOCOOLER_BOOST_CONFIG_FILE);
//    std::vector<std::string> vector_config;
//    std::string line;
//    if (inFile.is_open()){
//        while (getline(inFile, line, '"')){
//            if(line.find('#') == 0) continue;
//                vector_config.push_back(line);
//        }
//    }
//    for (int i=20; i < vector_config.size(); i++) {
//        std::cout << vector_config[i] << "-i";
//    }
//
//    std::ifstream vector_config_file("AUTOCOOLER_BOOST_CONFIG_FILE");
//
//}

void setFanTable() {
    std::fstream file;
    file.open(EC_FILE, std::ios::binary | std::ios::out | std::ios::in);
    if (file.is_open()) {
        file.seekp(CPU_TEMP_0); file.put(0x37);
        file.seekp(CPU_TEMP_1); file.put(0x41);
        file.seekp(CPU_TEMP_2); file.put(0x46);
        file.seekp(CPU_TEMP_3); file.put(0x50);
        file.seekp(CPU_TEMP_4); file.put(0x52);
        file.seekp(CPU_TEMP_5); file.put(0x55);

        file.seekp(CPU_FAN_SPEED_0); file.put(0x00);
        file.seekp(CPU_FAN_SPEED_1); file.put(0x14);
        file.seekp(CPU_FAN_SPEED_2); file.put(0x28);
        file.seekp(CPU_FAN_SPEED_3); file.put(0x3B);
        file.seekp(CPU_FAN_SPEED_4); file.put(0x50);
        file.seekp(CPU_FAN_SPEED_5); file.put(0x64);

        file.seekp(GPU_TEMP_0); file.put(0x37);
        file.seekp(GPU_TEMP_1); file.put(0x41);
        file.seekp(GPU_TEMP_2); file.put(0x46);
        file.seekp(GPU_TEMP_3); file.put(0x50);
        file.seekp(GPU_TEMP_4); file.put(0x52);
        file.seekp(GPU_TEMP_5); file.put(0x55);

        file.seekp(GPU_FAN_SPEED_0); file.put(0x00);
        file.seekp(GPU_FAN_SPEED_1); file.put(0x14);
        file.seekp(GPU_FAN_SPEED_2); file.put(0x28);
        file.seekp(GPU_FAN_SPEED_3); file.put(0x3B);
        file.seekp(GPU_FAN_SPEED_4); file.put(0x50);
        file.seekp(GPU_FAN_SPEED_5); file.put(0x64);

        file.seekp(BATTERY_CHARGING_THRESHOLD); file.put(0x5A);
        file.seekp(FAN_MODE); file.put(0x8C);

            std::cout << "The table was successfully written" << std::endl;
        }
        sleep(1);
        file.close();
}

int control_ACB() {

    std::fstream file;
    file.open(EC_FILE, std::ios::binary | std::ios::out | std::ios::in);
    uint16_t realTemp[] = {0, 0};

    while (file.is_open()){
        file.seekg(REALTIME_CPU_TEMP);
        file.read((char *) &realTemp[0], sizeof(uint16_t));
        file.seekp(REALTIME_GPU_TEMP);
        file.read((char *) &realTemp[1], sizeof(uint16_t));
        if (realTemp[0] > CPU_MAX_TEMP || realTemp[1] > GPU_MAX_TEMP) {
            file.seekp(COOLER_BOOST) ;
            file.put(COOLER_BOOST_ON);
            }
        if (realTemp[0] < (CPU_MAX_TEMP - 0x10) || realTemp[1] < (GPU_MAX_TEMP - 0x10)) {
            file.seekp(COOLER_BOOST)  ;
            file.put(COOLER_BOOST_OFF);
        }
        file.close();
    }
    usleep(500);
    return 0;
}


int main(int argc, char *argv[])
{
// Need to support write to EC
    system("modprobe ec_sys write_support=1");

// readACBConfig();
    setFanTable();
    while (true) {
        control_ACB();
    }
}
