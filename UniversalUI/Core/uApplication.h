//  uApplication.h   first written by Seb H in February 2023
//
//  uApplication is the base class for UUI applications. You
//  should create a subclass for your app, overriding the
//  functions that you would like to make use of.

#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <string>

class UUI uApplication {
public:
    std::string name;
    int majorVersion;
    int minorVersion;
    uApplication(const char* initName = "UniversalUI Application", int initMajorVersion = 1, int initMinorVersion = 0);
    virtual void FinishedLaunching();
    virtual void WillQuit();
    virtual bool ShouldQuit();
};

#endif
