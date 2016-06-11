#ifndef GPUINFO_H
#define GPUINFO_H
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
#include <QString>
#include <QDebug>
#include <iostream>
//Константы
#define NVAPI_MAX_PHYSICAL_GPUS   64
#define NVAPI_MAX_USAGES_PER_GPU  34
#define NVAPI_MAX_THERMAL_SENSORS_PER_GPU 3
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

//Класс, описывающий GPU
class GPUInfo
{

    char *nameGPU;              //название GPU
    int temperature;            //Температура GPU
    int GPUUsage;               //текущая загруженнсть GPU
//структуры, содержащие информацию о GPU
typedef enum
{
    NVAPI_THERMAL_TARGET_GPU           = 1,
    NVAPI_THERMAL_TARGET_MEMORY        = 2,
    NVAPI_THERMAL_TARGET_POWER_SUPPLY  = 4,
    NVAPI_THERMAL_TARGET_BOARD         = 8,
    NVAPI_THERMAL_TARGET_VCD_BOARD     = 9,
    NVAPI_THERMAL_TARGET_VCD_INLET     = 10,
    NVAPI_THERMAL_TARGET_VCD_OUTLET    = 11,
    NVAPI_THERMAL_TARGET_ALL           = 15,
    NVAPI_THERMAL_TARGET_UNKNOWN       = -1,
} NV_THERMAL_TARGET;

typedef struct
{
    int version;
    int count;
    struct
    {
        int controller;
        int defaultMinTemp;
        int defaultMaxTemp;
        int currentTemp;
        NV_THERMAL_TARGET target;
    } sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU];

} NV_GPU_THERMAL_SETTINGS;

public :
    //методы, используемые для получения информации о GPU
    typedef int *(*NvAPI_QueryInterface_t)(unsigned int offset);
    typedef int (*NvAPI_Initialize_t)();
    typedef int (*NvAPI_EnumPhysicalGPUs_t)(int **handles, int *count);
    typedef int (*NvAPI_GPU_GetUsages_t)(int *handle, unsigned int *usages);
    typedef int (*NvAPI_GPU_GetThermalSettings_t)(int *handle, int sensorIndex, NV_GPU_THERMAL_SETTINGS *temp);

    QString getNameGPU();   //метод получения навания GPU
    void UpdateState();     //метод обновления состояния GPU
    int getTemperature();   //метод получения температуры GPU
    int getGPUUsage();      //метод получения загруженности GPU

};

#endif // GPUINFO_H
