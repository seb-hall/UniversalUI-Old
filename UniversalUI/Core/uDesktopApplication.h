//  uDesktopApplication.h   first written by Seb H in February 2023
//
//  uDesktopApplication is a subclass of uApplication, using a
//  window manager to provide multi-window and background
//  processing functionality. It is supported on desktop platforms
//  such as Windows, macOS and Linux-GTK, but NOT on platforms such
//  as iOS, Android or Web.

// CROSS-PLATFORM

#ifndef UDESKTOPAPPLICATION_H
#define UDESKTOPAPPLICATION_H

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
#include <UniversalUI/Core/uWindowManager.h>
#include <string>

class UUI uDesktopApplication: public uApplication {
public:

    //  uDesktopApplication provides a single pointer
    //  for an instance of uWindowController. This should be insantiated
    //  before uuiMain() is called, otherwise the application will quit.
    uWindowManager* windowManager;

    //  init application, taking name and version number as parameters.
    uDesktopApplication(const char* initName, int initMajorVersion, int initMinorVersion);

    //  called when the application has finished launching - i.e only
    //  once the UniversalUI environment has been initialised and the 
    //  app can actually start doing stuff like making windows!
    virtual void FinishedLaunching(int argc, char* argv[]) override { }
    
    //  called when the application has been requested to quit by the operating
    //  system. use only for last-second mandatory operations.
    virtual void WillQuit() override { }
    
    //  called when the user has requested the application to quit. e.g by pressing
    //  ctrl/cmd + q. this function returns true by default and will call the willQuit
    //  function when it does. if you don't want to quit the application i.e if there
    //  is unsaved work, there should be some kind of visual feedback to the user 
    //  that this is the case.
    virtual bool ShouldQuit() { return true; }
};

#endif
