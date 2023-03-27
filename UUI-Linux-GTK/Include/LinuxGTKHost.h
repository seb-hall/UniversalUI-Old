//  LinuxGTKHost.h   first written by sebhall in March 2023
//
//  LinuxGTKHost is a subclass of CoreHost, designed as the
//  standard Linux host for UniversalUI applications. 
//
//  LinuxGTKHost is the module directly interfacing with GTK
//  and GDK, meaning it is responsible for the creation of 
//  windows and contexts, window events, input events and the
//  control of OpenGL contexts.

#ifndef LINUXGTKHOST_H
#define LINUXGTKHOST_H

#define UUI

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Host/CoreHost.h>
#include <string>

class UUI LinuxGTKHost: public CoreHost {

public:
    bool TestEnvironment();
    int main() override;
    void ShowWindow(uWindow* window) override;
    void SetTitle(uWindow* window, std::string title) override;
};




#endif