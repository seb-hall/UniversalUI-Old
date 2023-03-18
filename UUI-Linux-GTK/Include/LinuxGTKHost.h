//  LinuxGTKHost.h   first written by sebhall in March 2023
//
//  LinuxGTKHost is a subclass of CoreHost, designed as the
//  standard Linux host for UniversalUI applications.

#ifndef LINUXGTKHOST_H
#define LINUXGTKHOST_H

#define UUI

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Host/CoreHost.h>
#include <string>

class UUI LinuxGTKHost: public CoreHost {

public:
    bool TestEnvironment() override;
    int main() override;
    void ShowWindow(uWindow* window) override;
    void SetTitle(uWindow* window, std::string title) override;
};




#endif