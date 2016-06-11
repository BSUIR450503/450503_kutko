#include "cpuinfo.h"

char * CPUInfo::getNameProcessor()
{
    int CPUInfo[4] = {-1};
           unsigned   nExIds, i =  0;
           //Получtение информацию, связанную с каждым расширением ID.
           __cpuid(CPUInfo, 0x80000000);
           nExIds = CPUInfo[0];
           for (i=0x80000000; i<=nExIds; ++i)
           {
               __cpuid(CPUInfo, i);
               // Interpret CPU brand string
               if  (i == 0x80000002)
                   memcpy(this->CPUBrandString, CPUInfo, sizeof(CPUInfo));
               else if  (i == 0x80000003)
                   memcpy(this->CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
               else if  (i == 0x80000004)
                   memcpy(this->CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
           }
       return CPUBrandString;
}

int CPUInfo::getNumbCores()
{
    SYSTEM_INFO sysInfo;                        //стуктура, содердащая информацию о ЦП
    GetSystemInfo(&sysInfo);                    //получение информации о ЦП
    numbOfCores = sysInfo.dwNumberOfProcessors; //количество ядер ЦП
    return numbOfCores;
}

int CPUInfo::getTotalSizeRAM()
{
    MEMORYSTATUSEX statex;                          //структура содержащая информацию об ОЗУ
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);                  //получение иофрмации об ОЗУ
    totalSizeRam = (statex.ullTotalPhys/1024)/1024; //перевод в мегабайты
    return totalSizeRam;
}

ULONGLONG CPUInfo::ft2ull(FILETIME &ft)
{
    ULARGE_INTEGER ul;
    ul.HighPart = ft.dwHighDateTime;
    ul.LowPart = ft.dwLowDateTime;
    return ul.QuadPart;
}

int CPUInfo::getCPUUsage()
{
        //переменные, содержащие системное время
        FILETIME idle, prev_idle;
        FILETIME kernel, prev_kernel;
        FILETIME user, prev_user;
        //получение времени
        GetSystemTimes(&prev_idle, &prev_kernel, &prev_user);
        Sleep(1000);
        GetSystemTimes(&idle, &kernel, &user);       
        //расчет загруженности ЦП в процентах
        ULONGLONG sys = (ft2ull(user) - ft2ull(prev_user)) +
        (ft2ull(kernel) - ft2ull(prev_kernel));
        currentCPUUsage = int((sys - ft2ull(idle) + ft2ull(prev_idle)) * 100.0 / sys);
        prev_idle = idle;
        prev_kernel = kernel;
        prev_user = user;
        return currentCPUUsage;
}

int CPUInfo::getCurrentRamUsage()
{
    MEMORYSTATUSEX stats;                //структура содержащая информацию об ОЗУ
    stats.dwLength = sizeof(stats);
    GlobalMemoryStatusEx(&stats);       //получение иофрмации об ОЗУ
   // __int64 memoryUsed;
    currentRAMUsage=stats.dwMemoryLoad; //получение текущей нагрузки ОЗУ
    return currentRAMUsage;
}
