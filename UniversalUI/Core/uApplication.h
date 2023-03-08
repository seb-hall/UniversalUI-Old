//  uApplication.h   first written by Seb H in February 2023
//
//  uApplication is the base class for UUI applications. You
//  should ALWAYS subclass it for your app, overriding the
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

    // user application name
    std::string name;

    // major version number i.e v1.x
    int majorVersion;

    // minor version number i.e vx.1
    int minorVersion;

    //  init application, taking name and version number as parameters.
    uApplication(const char* initName, int initMajorVersion, int initMinorVersion);

    //  called when the application has finished launching - i.e only
    //  once the UniversalUI environment has been initialised and the 
    //  app can actually start doing stuff like making windows!
    virtual void FinishedLaunching(int argc, char* argv[]);

    //  called when the application has been requested to quit by the operating
    //  system. use only for last-second mandatory operations.
    virtual void WillQuit();

    //  called when the user has requested the application to quit. e.g by pressing
    //  ctrl/cmd + q. this function returns true by default and will call the willQuit
    //  function when it does. if you don't want to quit the application i.e if there
    //  is unsaved work, there should be some kind of visual feedback to the user 
    //  that this is the case.
    virtual bool ShouldQuit();

};

#endif
