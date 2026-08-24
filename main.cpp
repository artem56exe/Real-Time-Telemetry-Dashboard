#include <iostream>
#include <Windows.h>

class CollectSystemData {

    private: 
        
        MEMORYSTATUSEX statex;
    public:
    CollectSystemData() {};

    void CollectData() {
        
        statex.dwLength = sizeof (statex);

        
        GlobalMemoryStatusEx (&statex);

    std::cout << "Used: " << statex.dwMemoryLoad  << "%\n";
    std::cout << "Total: " << statex.ullTotalPhys << " bytes\n";
    std::cout << "Free: " << statex.ullAvailPhys << " bytes\n";

    };

};

int main() {
    CollectSystemData collector;
    collector.CollectData();
    return 0;
}