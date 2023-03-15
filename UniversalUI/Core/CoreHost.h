//  CoreHost.h   first written by Seb H in March 2023
//
//  CoreHost is a an abstraction of the various platform
//  -specific hosts for UniversalUI. They must all conform
//  to it and this allows UniversalUI modules to access
//  the same information across all platforms.

// HOST-DEFINED

#ifndef COREHOST_H
#define COREHOST_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

class uApplication;
class uWindowController;
class uWindow;
class aRenderer;
class uInterfaceManager;
class uFileManager;

class UUI CoreHost {
public:

    uApplication* app;
    uWindowController* windowController;
    aRenderer* renderer;
    uInterfaceManager* interfaceManager;
    uFileManager* fileManager;

    // return true if environment passes all checks
    virtual bool TestEnvironment();

    // main loop function
    virtual int main();

    // Show uWindow
    virtual void ShowWindow(uWindow* window);

};



#endif
