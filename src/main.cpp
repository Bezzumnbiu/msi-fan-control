#include "defines.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <algorithm>
using namespace std;

string GetStdoutFromCommand(string cmd) {

  string data;
  FILE * stream;
  char buffer[256];

  stream = popen(cmd.c_str(), "r");

  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, 256, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}


char *convertToChar(const std::string & s)
{
   char *pc = new char[s.size()+1];
   std::strcpy(pc, s.c_str());
   return pc;
}

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void setFanTable() {
    std::fstream file;
    file.open(EC_FILE, std::ios::binary | std::ios::out | std::ios::in);
    if (file.is_open()) {

        file.seekp(CPU_TEMP_0); file.put(0x37);
        file.seekp(CPU_TEMP_1); file.put(0x41);
        file.seekp(CPU_TEMP_2); file.put(0x47);
        file.seekp(CPU_TEMP_3); file.put(0x4C);
        file.seekp(CPU_TEMP_4); file.put(0x50);
        file.seekp(CPU_TEMP_5); file.put(0x52);

        file.seekp(CPU_FAN_SPEED_0); file.put(0x00);
        file.seekp(CPU_FAN_SPEED_1); file.put(0x14);
        file.seekp(CPU_FAN_SPEED_2); file.put(0x28);
        file.seekp(CPU_FAN_SPEED_3); file.put(0x3B);
        file.seekp(CPU_FAN_SPEED_4); file.put(0x50);
        file.seekp(CPU_FAN_SPEED_5); file.put(0xff);

        file.seekp(GPU_TEMP_0); file.put(0x37);
        file.seekp(GPU_TEMP_1); file.put(0x41);
        file.seekp(GPU_TEMP_2); file.put(0x47);
        file.seekp(GPU_TEMP_3); file.put(0x4C);
        file.seekp(GPU_TEMP_4); file.put(0x50);
        file.seekp(GPU_TEMP_5); file.put(0x52);

        file.seekp(GPU_FAN_SPEED_0); file.put(0x00);
        file.seekp(GPU_FAN_SPEED_1); file.put(0x14);
        file.seekp(GPU_FAN_SPEED_2); file.put(0x28);
        file.seekp(GPU_FAN_SPEED_3); file.put(0x3B);
        file.seekp(GPU_FAN_SPEED_4); file.put(0x50);
        file.seekp(GPU_FAN_SPEED_5); file.put(0xff);


            std::cout << "The table was successfully written" << std::endl;
        }
        sleep(1);
        file.close();
}

int control_ACB(const std::vector<std::string> fileBuffer) {

//    uint16_t realTemp[fileBuffer.size()];
    std::vector<char*> realTemp;
    std::transform(fileBuffer.begin(), fileBuffer.end(), std::back_inserter(realTemp), convertToChar);

    int realIntTemp[realTemp.size()];
    for (int i=0; i < realTemp.size(); i++){
        std::ifstream hwmonFile(realTemp[i]);
        while(hwmonFile >> realIntTemp[i]){
        }
    }
    for (int i=0; i < realTemp.size()-1;i++){
        realIntTemp[i] = realIntTemp[i]/1000;
    }

    bool flagState = 0;
    std::vector<bool> boolTempTable;

    for(int i=0; i<realTemp.size()-1; i++){
        boolTempTable.push_back(realIntTemp[i] > SYSTEM_MAX_TEMP);
        if (boolTempTable[i]){flagState = 1;}
        std::cout << boolTempTable[i] << std::endl;
    }
//    for(int i=0; i<realTemp.size()-1; i++){
//        std::cout << boolTempTable[i] << std::endl;
//    }
    //Open EC file from debugfs and initialize realTemp array with values of... Real Temperatures?
    std::fstream controllerFile;
    controllerFile.open(EC_FILE, std::ios::binary | std::ios::out);

    while (controllerFile.is_open()){
        // Move to Cooler Boost address
        controllerFile.seekp(COOLER_BOOST);

        if (flagState == 1){controllerFile.put(COOLER_BOOST_ON) ;}
        if (flagState == 0){controllerFile.put(COOLER_BOOST_OFF);}
        controllerFile.close();
    }
    sleep(1);
    return 0;
}


int main(int argc, char *argv[]){
// Need to support write to EC, checks kernel modules if have errors
    int counter;
    system("modprobe ec_sys write_support=1");
    std::string str = GetStdoutFromCommand("ls /sys/class/hwmon/*/temp*_input");;
    std::string delimiter = "\n";
    std::vector<std::string> v = split (str, delimiter);
    std::vector<std::string> tempBuffer;

    for (string i : v){
        tempBuffer.push_back(i);
    }

    while (true) {
        control_ACB(tempBuffer);
    }


    return 0;



// readACBConfig();
    // Write EC table to works cooler fine
    //setFanTable();
    //Enable infilite loop...






}
