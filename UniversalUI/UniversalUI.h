//  UniversalUI.h   first written by sebhall in February 2023
//
//  Welcome to the UniversalUI framework! UUI has been designed
//  to be a lightweight, efficient and user-friendly SDK. Use
//  UUI to build apps for every major platform, including iOS,
//  iPadOS, Android, Windows, macOS, ChromeOS, Linux and BSD.
//
//  For documentation, sample code and more information please
//  see our website.
//
//  UniversalUI is released under the MIT Software Licence,
//  allowing you to use the framework for any project,
//  including commercial and proprietary software, all without
//  crediting the UniversalUI framework directly if not desired.

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

//  Run UniversalUI. This function takes an instance of uApplication
//  but it should always be either an instance of uDesktopApplication
//  or uSimpleApplication, depending on the platform and the kind of
//  app you're designing. The framework will first check your app and
//  the installation for compatibiltiy, returning an error code (defined
//  later in this file) if there are any issues. If all is well, the
//  framework will then start the main loop and call the FinishedLaunching
//  method on your uApplication instance.
int UUI UniversalUI(uApplication* userApp);

#define EXIT_SUCCESS    0       //  normal exit conditions
#define APP_TYPE_ERROR  1       //  app type unsupported (e.g an instance of uDesktopApplication being used on iOS)
#define APP_CLASS_ERROR 2       //  app is an instance of uApplication and not uDesktopApplication or uSimpleApplication
#define OPENGL_ERROR    3       //  platform doesn't support OpenGL to the required level (for non-Apple platforms)
#define METAL_ERROR     4       //  platform doesn't support Metal (for Apple platforms)

#endif
