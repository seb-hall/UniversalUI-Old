//  uApplication.h   first written by Seb H in February 2023
//
//  uApplication is the base class for all UUI applications.
//  It shouldn't be subclassed directly, instead you should
//  use either uDesktopApplication for dekstop-class apps
//  on operating systems such as Windows, macOS and Linux, or
//  uSimpleAppliction for single-view apps on platforms such
//  as iOS and Web.

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

class CoreHost;

class UUI uApplication {
public:

    //  forward-defined pointer for host access.
    CoreHost* host;

    //  user application name
    std::string name;

    //  major version number i.e v1.x
    int majorVersion;

    //  minor version number i.e vx.1
    int minorVersion;

    //  called when the application has finished launching - i.e only
    //  once the UniversalUI environment has been initialised and the 
    //  app can actually start doing stuff like making windows!
    virtual void FinishedLaunching(int argc, char* argv[]);

    //  called when the application has been requested to quit by the operating
    //  system. use only for last-second mandatory operations.
    virtual void WillQuit();

};

#endif
