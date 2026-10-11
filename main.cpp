#include <iostream>
#include <Windows.h>
#include <pdh.h>
#include <vector>
#pragma comment(lib, "pdh.lib")


class CollectAllData {
    private:
        PDH_HQUERY phQuery;
        PDH_HCOUNTER phCounterCPUpercentage, phCounterDTpercentage, phCounterBYTESsent, phCounterBYTESreceived, phCounterGPUpercentage;
        PDH_FMT_COUNTERVALUE valueCPUpercentage, valueDTpercentage, valueGPUpercentage, valueBYTESsent, valueBYTESreceived;
        MEMORYSTATUSEX statex;
        DWORD bufferGPUsize, bufferBYTESsentSIZE, bufferBYTESreceivedSIZE;
        DWORD bufferGPUitemcount, bufferBYTESsentitemcount, bufferBYTESreceiveditemcount;
        double totalGPU = 0;
        double totalBYTESsent = 0;
        double totalBYTESreceived = 0;
    public:
            CollectAllData() {CollectData();}

        void CollectData() {
        
    statex.dwLength = sizeof (statex);
    BOOL status2 = GlobalMemoryStatusEx (&statex);

    PdhOpenQuery(NULL, 0, &phQuery);
    PdhAddEnglishCounterW(phQuery, L"\\Processor(_Total)\\% Processor Time", 0, &phCounterCPUpercentage);   // CPU usage
    PdhAddEnglishCounterW(phQuery, L"\\PhysicalDisk(_Total)\\% Disk Time", 0, &phCounterDTpercentage);      // Disk Activity
    PdhAddEnglishCounterW(phQuery, L"\\NBT Connection(*)\\Bytes Received/sec", 0, &phCounterBYTESreceived); // Bytes Received
    PdhAddEnglishCounterW(phQuery, L"\\NBT Connection(*)\\Bytes Sent/sec", 0, &phCounterBYTESsent);         // Bytes Sent
    PdhAddEnglishCounterW(phQuery, L"\\GPU Engine(*)\\Utilization Percentage", 0, &phCounterGPUpercentage); // Bytes Sent
    PdhCollectQueryData(phQuery);
    Sleep(500);
    PdhCollectQueryData(phQuery);
    PdhGetFormattedCounterValue(phCounterCPUpercentage, PDH_FMT_DOUBLE, NULL, &valueCPUpercentage);
    PdhGetFormattedCounterValue(phCounterDTpercentage, PDH_FMT_DOUBLE, NULL, &valueDTpercentage);

    PdhGetFormattedCounterArrayW(phCounterGPUpercentage, PDH_FMT_DOUBLE, &bufferGPUsize, &bufferGPUitemcount, NULL);
    PdhGetFormattedCounterArrayW(phCounterBYTESsent, PDH_FMT_DOUBLE, &bufferBYTESsentSIZE, &bufferBYTESsentitemcount, NULL);
    PdhGetFormattedCounterArrayW(phCounterBYTESreceived, PDH_FMT_DOUBLE, &bufferBYTESreceivedSIZE, &bufferBYTESreceiveditemcount, NULL);

    std::vector<BYTE> bufferGPUpercentage(bufferGPUsize);
    std::vector<BYTE> bufferBYTESsent(bufferBYTESsentSIZE);
    std::vector<BYTE> bufferBYTESreceived(bufferBYTESreceivedSIZE);

    auto itemsGPU = reinterpret_cast<PPDH_FMT_COUNTERVALUE_ITEM_W>(bufferGPUpercentage.data());
    auto itemsBYESsent = reinterpret_cast<PPDH_FMT_COUNTERVALUE_ITEM_W>(bufferBYTESsent.data());
    auto itemsBYESreceived = reinterpret_cast<PPDH_FMT_COUNTERVALUE_ITEM_W>(bufferBYTESreceived.data());

    PdhGetFormattedCounterArrayW(phCounterGPUpercentage, PDH_FMT_DOUBLE, &bufferGPUsize, &bufferGPUitemcount, itemsGPU);
    PdhGetFormattedCounterArrayW(phCounterBYTESsent, PDH_FMT_DOUBLE, &bufferBYTESsentSIZE, &bufferBYTESsentitemcount, itemsBYESsent);
    PdhGetFormattedCounterArrayW(phCounterBYTESreceived, PDH_FMT_DOUBLE, &bufferBYTESreceivedSIZE, &bufferBYTESreceiveditemcount, itemsBYESreceived);


    for (int i = 0; i < bufferGPUitemcount; i++) {totalGPU += itemsGPU[i].FmtValue.doubleValue;}
    for (int i = 0; i < bufferBYTESsentitemcount; i++) {totalBYTESsent += itemsBYESsent[i].FmtValue.doubleValue;}
    for (int i = 0; i < bufferBYTESreceiveditemcount; i++) {totalBYTESreceived += itemsBYESreceived[i].FmtValue.doubleValue;}


    std::cout << "CPU Usage: " << valueCPUpercentage.doubleValue << "%" << std::endl;
    std::cout << "GPU Usage: " << totalGPU << "%" << std::endl;
    std::cout << "Disk Activity Usage: " << valueDTpercentage.doubleValue << "%" << std::endl;
    std::cout << "RAM Used: " << statex.dwMemoryLoad  << "%" << std::endl;
    std::cout << "RAM Total: " << (statex.ullTotalPhys)/(1024*1024) << "MB" << std::endl;
    std::cout << "RAM Free: " << (statex.ullAvailPhys)/(1024*1024) << "MB" << std::endl;;
    std::cout << "Bytes Received: " << totalBYTESreceived << "bytes" << std::endl;
    std::cout << "Bytes Sent: " << totalBYTESsent << "bytes" << std::endl;


    PdhCloseQuery(phQuery);
    
    }

};


int main() {
    CollectAllData collector;
    return 0;
}