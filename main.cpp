#include <iostream>
#include <Windows.h>
#include <iomanip>


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
    BOOL status = GetSystemTimes(&lpIdleTime,&lpKernelTime,&lpUserTime);

    uli1.LowPart  = lpIdleTime.dwLowDateTime;
    uli1.HighPart = lpIdleTime.dwHighDateTime;

    uli2.LowPart  = lpKernelTime.dwLowDateTime;
    uli2.HighPart = lpKernelTime.dwHighDateTime;

    uli3.LowPart  = lpUserTime.dwLowDateTime;
    uli3.HighPart = lpUserTime.dwHighDateTime;

    double a1 = uli2.QuadPart + uli3.QuadPart - uli1.QuadPart;
    double a2 = uli2.QuadPart + uli3.QuadPart;

    std::cout << "\n\nCPU Data" << std::endl;
    std::cout << "Used: "<< std::fixed << std::setprecision(2) << (a1/a2)*100 << "%" ;

    }
};


class CollectRAMData {

    private: 
        MEMORYSTATUSEX statex;
    public:
    CollectRAMData() {CollectRAMInfo();};

    void CollectRAMInfo() {

        statex.dwLength = sizeof (statex);
        GlobalMemoryStatusEx (&statex);

    std::cout << "\n\nRAM Data" << std::endl;
    std::cout << "Used: " << statex.dwMemoryLoad  << std::endl;
    std::cout << "Total: " << statex.ullTotalPhys << std::endl;
    std::cout << "Free: " << statex.ullAvailPhys;

    };

};

int main() {
    CollectRAMData collector;
    CollectCPUData collector1;
    return 0;
}