//  UniversalUI.h   first written by Seb H in February 2023
//
//  Welcome to the UniversalUI framework! UUI has been designed
//  to be a lightweight, efficient and user-friendly SDK. Use
//  UUI to build apps for every major platform, including iOS,
//  iPadOS, Android, Windows, macOS, ChromeOS, Linux and BSD.
//
//  For documentation, sample code and more information please
//  see our website.
//
//  UniversalUI is released under the Unlicence Software
//  Licence, allowing you to use the framework for any project,
//  including commercial and proprietary software, all without
//  referring to the UniversalUI framework directly.

//  UniversalUI.h contains the main interface with the framework
//  and should be included in your main.cpp file. All files in
//  this directory - "UniversalUI" are completely cross-platform.

#ifndef UNIVERSALUI_H
#define UNIVERSALUI_H

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

bool UUI UniversalUI(uApplication* userApp);
int UUI uuiMain(int argc, char * argv[]);


#endif
