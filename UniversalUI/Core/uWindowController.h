//  uWindowController.h   first written by Seb H in 2022
//
//  uWindowController controls window interaction.

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

class uWindow;

class UUI uWindowController {
public:
    std::vector<uWindow*> windows;
    uWindow* CreateWindow(std::string title, uSize size);
    uWindow* CreateWindowFromFile(std::string filePath);
    
};


#endif /* uWindowController_h */
