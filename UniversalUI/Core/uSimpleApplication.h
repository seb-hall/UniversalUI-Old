//  uSimpleApplication.h   first written by Seb H in February 2023
//
//  uSimpleApplication is a subclass of uApplication, using a
//  single window and intended for use on platforms such as iOS, 
//  Android and Web. It is supported on all platforms but only
//  recommended on desktop platforms for very basic use-cases.

// CROSS-PLATFORM

#ifndef USIMPLEAPPLICATION_H
#define USIMPLEAPPLICATION_H

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

class UUI uSimpleApplication: public uApplication {
public:

    //  uSimpleApplication provides a single pointer
    //  for an instance of uWindow. This should be insantiated
    //  before uuiMain() is called, otherwise the application 
    //  will quit.
    uWindow* window;

    //  init application, taking name and version number as parameters.
    uSimpleApplication(const char* initName, int initMajorVersion, int initMinorVersion);

    //  called when the application has finished launching - i.e only
    //  once the UniversalUI environment has been initialised and the 
    //  app can actually start doing stuff like making windows!
    virtual void FinishedLaunching(int argc, char* argv[]) override { }

    //  called when the application has been requested to quit by the operating
    //  system. use only for last-second mandatory operations.
    virtual void WillQuit() override { }
};

#endif
