#include "../Platform.h"
#include <iostream>
using namespace sablin;
using namespace std;

int main(){
    if(PlatformInfo::IsWindows()){
        std::cout << "Windows" << std::endl;
    }
    if(PlatformInfo::IsLinux()){
        std::cout << "Linux" << std::endl;
    }
    if(PlatformInfo::IsArch32()){
        std::cout << "Arch: 32" << std::endl;
    }
    if(PlatformInfo::IsArch64()){
        std::cout << "Arch: 64" << std::endl;
    }
    return 0;
}
