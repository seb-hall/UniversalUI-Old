//  CoreHost.h   first written by sebhall in late 2022
//
//  CoreHost is a an abstraction of the various platform
//  -specific hosts for UniversalUI. They must all conform
//  to it and this allows UniversalUI modules to access
//  the same information across all platforms.

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

#include <string>

//  forward class definitions. Some of these use
//  pointers to CoreHost so these are to avoid
//  include loops.

class uApplication;
class uWindow;
class CoreAngelo;

enum uApplicationType {
    invalid,
    simple,
    desktop
};

class UUI CoreHost {
public:

    //  pointers to single-instance modules
    uApplication* app;
    uApplicationType appType;
    CoreAngelo* angelo;

    // main loop function
    virtual int main();

    // Show uWindow
    virtual void ShowWindow(uWindow* window);

    // Set uWindow title
    virtual void SetTitle(uWindow* window, std::string title);

};



#endif
