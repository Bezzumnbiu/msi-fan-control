#include "./defines.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>
#include <vector>

std::vector<std::string>
checkHardwarePaths() {
  std::vector<std::string> tempBuffer_paths;
  std::string path = "/sys/class/hwmon/";
  std::string str;
  std::regex reg("temp[0-9]+_input");

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(path)) {
    for (const auto &entry_new :
         std::filesystem::recursive_directory_iterator(entry.path())) {
      str = entry_new.path();
      if (std::regex_search(str, reg) == 1)
        tempBuffer_paths.push_back(str);
    }
  }
  return tempBuffer_paths;
}

void setFanTable() {
  std::fstream file;
  file.open(EC_FILE, std::ios::binary | std::ios::out | std::ios::in);
  if (file.is_open()) {

    file.seekp(CPU_TEMP_0);file.put(0x3A);
    file.seekp(CPU_TEMP_1);file.put(0x41);
    file.seekp(CPU_TEMP_2);file.put(0x47);
    file.seekp(CPU_TEMP_3);file.put(0x4C);
    file.seekp(CPU_TEMP_4);file.put(0x50);
    file.seekp(CPU_TEMP_5);file.put(0x52);

    file.seekp(CPU_FAN_SPEED_0);file.put(0x00);
    file.seekp(CPU_FAN_SPEED_1);file.put(0x14);
    file.seekp(CPU_FAN_SPEED_2);file.put(0x28);
    file.seekp(CPU_FAN_SPEED_3);file.put(0x3B);
    file.seekp(CPU_FAN_SPEED_4);file.put(0x50);
    file.seekp(CPU_FAN_SPEED_5);file.put(0x64);

    file.seekp(GPU_TEMP_0);file.put(0x3A);
    file.seekp(GPU_TEMP_1);file.put(0x41);
    file.seekp(GPU_TEMP_2);file.put(0x47);
    file.seekp(GPU_TEMP_3);file.put(0x4C);
    file.seekp(GPU_TEMP_4);file.put(0x50);
    file.seekp(GPU_TEMP_5);file.put(0x52);

    file.seekp(GPU_FAN_SPEED_0);file.put(0x00);
    file.seekp(GPU_FAN_SPEED_1);file.put(0x14);
    file.seekp(GPU_FAN_SPEED_2);file.put(0x28);
    file.seekp(GPU_FAN_SPEED_3);file.put(0x3B);
    file.seekp(GPU_FAN_SPEED_4);file.put(0x50);
    file.seekp(GPU_FAN_SPEED_5);file.put(0x64);

    std::cout << "The table was successfully written" << std::endl;
  }
  sleep(1);
}

int control_ACB(const std::vector<std::string> &fileBuffer) {
  bool flagState = 0;
  for (int i = 0; i < fileBuffer.size(); i++) {
    std::ifstream hwmonFile(fileBuffer[i]);
    int tmpTemp;
    while(hwmonFile >> tmpTemp){
      if((tmpTemp / 1000) > SYSTEM_MAX_TEMP){
        flagState = 1;
        break;
      }
    }
  }

  std::fstream controllerFile(EC_FILE, std::ios::binary | std::ios::out);
  while (controllerFile.is_open()) {
    // Move to Cooler Boost address
    controllerFile.seekp(COOLER_BOOST);

    if (flagState == 1) {
      controllerFile.put(COOLER_BOOST_ON);
    }
    if (flagState == 0) {
      controllerFile.put(COOLER_BOOST_OFF);
    }
    controllerFile.close();
  }

  sleep(1);
  return 0;
}

int main(int argc, char *argv[]) {
  // Need to support write to EC, checks kernel modules if have errors
  system("modprobe ec_sys write_support=1");
  setFanTable();
  while (true) {
    control_ACB(checkHardwarePaths());
  }
  return 0;
}
