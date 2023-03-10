//  uWindowController.h   first written by Seb H in 2022
//
//  uWindowController controls window operation for instances
//  of uDesktopApplication.

#ifndef UWINDOWCONTROLLER_H
#define UWINDOWCONTROLLER_H

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

enum UUI uWindowCreationOptions {
    standard,
    fixedSize
};

class UUI uWindowController {
public:
    std::vector<uWindow*> windows;
    uWindow* CreateWindow(std::string title, uSize size, uWindowCreationOptions options = standard);
    uWindow* CreateWindowFromFile(std::string filePath);
};


#endif /* uWindowController_h */
