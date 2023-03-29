//  WinHost.h   first written by sebhall in March 2023
//
//  WinHost is a subclass of CoreHost, designed as the
//  standard Windows host for UniversalUI applications.
//
//  WinHost is the module directly interfacing with 
//  Win32, meaning it is responsible for the creation
//  of windows and contexts, window events, and the
//  conrol of OpenGL contexts.

#ifndef WINHOST_H
#define WINHOST_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Host/CoreHost.h>
#include <string>

class UUI WinHost: public CoreHost {

public:
    bool TestEnvironment();
    int main() override;
    void ShowWindow(uWindow* window) override;
    void SetTitle(uWindow* window, std::string title) override;
};




#endif