#include <iostream>
#include <Windows.h>

class CollectSystemData {

    private: 
        // structure hold system memmory
        MEMORYSTATUSEX statex;
    public:
    CollectSystemData() {};

    void CollectData() {
        // define the length
        statex.dwLength = sizeof (statex);

        // retrieves data
        GlobalMemoryStatusEx (&statex);

    std::cout << "Used: " << statex.dwMemoryLoad  << "%\n";             // percentage of RAM usage
    std::cout << "Total: " << statex.ullTotalPhys << " bytes\n";        // Total amount of memmory (bytes)
    std::cout << "Free: " << statex.ullAvailPhys << " bytes\n";         // Total amount of free memmory (bytes)

    };

};

int main() {
    CollectSystemData collector;
    collector.CollectData();
    return 0;
}