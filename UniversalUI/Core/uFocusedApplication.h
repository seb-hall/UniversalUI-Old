//  uFocusedApplication.h   first written by Seb H in February 2023
//
//  uFocusedApplication is a subclass of uApplication, specifically
//  designed for single-window applications such as mobile apps and
//  web-based applications.

#ifndef UFOCUSEDAPPLICATION_H
#define UFOCUSEDAPPLICATION_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uWindow.h>
#include <string>

class UUI uFocusedApplication: public uApplication {
public:
    uWindow* window;
    uFocusedApplication(const char* initName = "UniversalUI Application", int initMajorVersion = 1, int initMinorVersion = 0);
    void FinishedLaunching(int argc, char* argv[]) override;
};

#endif
