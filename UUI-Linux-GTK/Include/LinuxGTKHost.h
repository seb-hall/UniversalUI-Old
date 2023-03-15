//  LinuxGTKHost.h   first written by Seb H in March 2023
//
//  LinuxGTKHost is a subclass of CoreHost, designed as the
//  standard Linux host for UniversalUI applications.

#ifndef LINUXGTKHOST_H
#define LINUXGTKHOST_H

#define UUI

#include <UniversalUI/Core/CoreHost.h>

class UUI LinuxGTKHost: public CoreHost {

public:
    bool TestEnvironment() override;
    int main() override;

};




#endif