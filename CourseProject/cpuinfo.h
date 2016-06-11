#ifndef CPUINFO_H
#define CPUINFO_H

#include <Windows.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include "psapi.h"
#include <tchar.h>
#include <iostream>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>

using namespace std;
//класс, описывающий информацию о CPU
class CPUInfo {

private:
    int numbOfCores;                 //количество ядер ЦП
    char CPUBrandString[0x40];       //название ЦП
    int totalSizeRam;                //размер оперативной памяти
    int currentCPUUsage;             //текущая загруженность ЦП
    __int64 currentRAMUsage;         //текущая используемая память

public:
    char* getNameProcessor();       //получение названия ЦП
    int getNumbCores();             //полученик количества ядер
    int getTotalSizeRAM();          //полечение размера ОЗУ
    int getCPUUsage();              //получение текущей загруженности ЦП
    int getCurrentRamUsage();       //получение текущей испольщуемой ОЗУ
     ULONGLONG ft2ull(FILETIME &ft);

};


#endif // CPUINFO_H
