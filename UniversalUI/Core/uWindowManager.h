//  uWindowManager.h   first written by Seb H in 2022
//
//  uWindowManager controls window operation for instances
//  of uDesktopApplication.

// CROSS-PLATFORM

#ifndef UWINDOWMANAGER_H
#define UWINDOWMANAGER_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <vector>
#include <string>
#include <UniversalUI/Core/CoreGeometry.h>
#include <UniversalUI/Core/uWindow.h>

enum uWindowCreationOptions {
    standard,
    fixedSize
};

class UUI uWindowManager {
public:
    std::vector<uWindow*> windows;
    uWindow* CreateWindow(std::string title, uSize size, uWindowCreationOptions options = standard);
    uWindow* CreateWindowFromFile(std::string filePath);
    virtual void WindowMoved(uWindow* window, uPoint toLocation);
    virtual void WindowResized(uWindow* window, uSize toSize);
};


#endif /* uWindowManager_h */
