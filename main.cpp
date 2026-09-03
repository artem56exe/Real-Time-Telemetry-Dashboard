#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <pdh.h>
#include <vector>

class CollectCPUData {
    private:

    ULARGE_INTEGER uli1;
    ULARGE_INTEGER uli2;
    ULARGE_INTEGER uli3;
    FILETIME lpIdleTime;
    FILETIME lpKernelTime;
    FILETIME lpUserTime;

    public: CollectCPUData() {CollectCPUInfo();};

    void CollectCPUInfo() {
    BOOL status1 = GetSystemTimes(&lpIdleTime,&lpKernelTime,&lpUserTime);

    if (status1) {

        uli1.LowPart  = lpIdleTime.dwLowDateTime;
        uli1.HighPart = lpIdleTime.dwHighDateTime;

        uli2.LowPart  = lpKernelTime.dwLowDateTime;
        uli2.HighPart = lpKernelTime.dwHighDateTime;

        uli3.LowPart  = lpUserTime.dwLowDateTime;
        uli3.HighPart = lpUserTime.dwHighDateTime;

        double a1 = uli2.QuadPart + uli3.QuadPart - uli1.QuadPart;
        double a2 = uli2.QuadPart + uli3.QuadPart;

        std::cout << "\n\nCPU Data" << std::endl;
        std::cout << "Used: "<< std::fixed << std::setprecision(2) << (a1/a2)*100 << "%" << "\n\n";}
    
    else {std::cout << "Error with gathering CPU Data.";}

    }
};


class CollectRAMData {

    private: 
        MEMORYSTATUSEX statex;
    public:
    CollectRAMData() {CollectRAMInfo();};

    void CollectRAMInfo() {

        statex.dwLength = sizeof (statex);
    BOOL status2 = GlobalMemoryStatusEx (&statex);

    if (status2) {
        std::cout << "\n\nRAM Data" << std::endl;
        std::cout << "Used: " << statex.dwMemoryLoad  << std::endl;
        std::cout << "Total: " << statex.ullTotalPhys << std::endl;
        std::cout << "Free: " << statex.ullAvailPhys;
        }

    else {std::cout << "Error with gathering RAM Data.";}

    };

};

class CollectGPUData {
    private:
        PDH_HQUERY phQuery;
        PDH_HCOUNTER phCounter;
        DWORD lpdwBufferSize = 0;
        DWORD lpdwItemCount = 0;
        double totalGPU = 0;
    public: 
        CollectGPUData() {CollectRAMInfo();};

    void CollectRAMInfo() {

PdhOpenQuery(NULL, 0, &phQuery);
    PdhAddEnglishCounterW(phQuery, L"\\GPU Engine(*)\\Utilization Percentage", 0, &phCounter);
    PdhCollectQueryData(phQuery);
    Sleep(1000);
    PdhCollectQueryData(phQuery);
    PdhGetFormattedCounterArrayW(phCounter, PDH_FMT_DOUBLE, &lpdwBufferSize, &lpdwItemCount, NULL);


    //needed because we get an arrays when requesting a counter(eg. GPU Engine(*))
    std::vector<BYTE> buffer(lpdwBufferSize);
    auto items =reinterpret_cast<PPDH_FMT_COUNTERVALUE_ITEM_W>(buffer.data());
    PdhGetFormattedCounterArrayW(phCounter, PDH_FMT_DOUBLE, &lpdwBufferSize, &lpdwItemCount, items);


    for (int i = 0; i < lpdwItemCount; i++) {totalGPU += items[i].FmtValue.doubleValue;}
    //

    std::cout << "GPU Usage: " << totalGPU << "%";

    PdhCloseQuery(phQuery);

    }
};









int main() {
    CollectRAMData collector0;
    CollectCPUData collector1;
    CollectGPUData collector2;
    return 0;
}