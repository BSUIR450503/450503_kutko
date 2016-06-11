#include "gpuinfo.h"

QString GPUInfo::getNameGPU()
{
    DISPLAY_DEVICE dd;                      //перенная, описывающая информация устройства
    memset(&dd, 0, sizeof(DISPLAY_DEVICE));
    dd.cb = sizeof(dd);
    EnumDisplayDevices(NULL, 0, &dd, 0);    //получение информации о GPU
    //перевод из wchar в QString
    QString str = QString::fromWCharArray(dd.DeviceString);
    return str;
}

void GPUInfo::UpdateState()
{
    HMODULE hmod = LoadLibraryA("nvapi.dll");   //загрузка бибиотеки о GPU
    // nvapi.dll Указатели на функции
    NvAPI_QueryInterface_t      NvAPI_QueryInterface     = NULL;
    NvAPI_Initialize_t          NvAPI_Initialize         = NULL;
    NvAPI_EnumPhysicalGPUs_t    NvAPI_EnumPhysicalGPUs   = NULL;
    NvAPI_GPU_GetUsages_t       NvAPI_GPU_GetUsages      = NULL;
    NvAPI_GPU_GetThermalSettings_t  NvAPI_GPU_GetThermalSettings = NULL;
    // Функция используется для получения других внутренних функций из библиотеки nvapi.dll
    NvAPI_QueryInterface = (NvAPI_QueryInterface_t) GetProcAddress(hmod, "nvapi_QueryInterface");
    // некоторые полезные внутренние функции, которые не экспортируемые nvapi.dll
    NvAPI_Initialize = (NvAPI_Initialize_t) (*NvAPI_QueryInterface)(0x0150E828);
    NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t) (*NvAPI_QueryInterface)(0xE5AC921F);
    NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t) (*NvAPI_QueryInterface)(0x189A1FDF);
    NvAPI_GPU_GetThermalSettings = (NvAPI_GPU_GetThermalSettings_t) (*NvAPI_QueryInterface)(0xE3640A56);
    if (NvAPI_Initialize == NULL || NvAPI_EnumPhysicalGPUs == NULL ||
        NvAPI_EnumPhysicalGPUs == NULL || NvAPI_GPU_GetUsages == NULL)
    {
        std::cout<< "Couldn't get functions in nvapi.dll" << std::endl;
    }
    // инициализация библиотеки NVAPI
    (*NvAPI_Initialize)();
    //инициализация переменных
    NV_GPU_THERMAL_SETTINGS nvgts;
    int          gpuCount = 0;
    int         *gpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { NULL };
    unsigned int gpuUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };
    gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;
    (*NvAPI_EnumPhysicalGPUs)(gpuHandles, &gpuCount);
    nvgts.version = sizeof(NV_GPU_THERMAL_SETTINGS) | (1<<16);
    nvgts.count = 0;
    nvgts.sensor[0].controller = -1;
    nvgts.sensor[0].target = NVAPI_THERMAL_TARGET_GPU;
    //получение температеры GPU
    (*NvAPI_GPU_GetThermalSettings)(gpuHandles[0], 0 ,&nvgts);
    //получение загруженности GPU
    (*NvAPI_GPU_GetUsages)(gpuHandles[0], gpuUsages);
   // int usage = gpuUsages[3];
    GPUUsage = gpuUsages[3];
    temperature = nvgts.sensor[0].currentTemp;
}

int GPUInfo::getTemperature()
{
    return temperature;
}

int GPUInfo::getGPUUsage()
{
    return GPUUsage;
}
