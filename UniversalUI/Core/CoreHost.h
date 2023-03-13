//  CoreHost.h   first written by Seb H in March 2023
//
//  CoreHost is a container class designed to provide
//  control and initialisation of the UniversalUI
//  environment, and to provide communication between 
//  UniversalUI modules and system services such as
//  graphics APIs and file management.

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
class aRenderer;
class uInterfaceManager;
class uFileManager;

class UUI CoreHost {
public:

    uApplication* app;
    aRenderer* renderer;
    uInterfaceManager* interfaceManager;
    uFileManager* fileManager;
};



#endif
