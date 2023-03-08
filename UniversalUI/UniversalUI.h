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


//  Initialise UniversalUI - checking for installation and user
//  aplication compatibility. If all is well, the function will
//  return true, and uuiMain should be called to start your app.
//  If the function returns false, there are errors in the host
//  platform i.e no OpenGL support or missing resources. The
//  int main() function should return at this point as UniversalUI
//  will not run correctly.

bool UUI UniversalUI(uApplication* userApp);

//  Run UniversalUI. This function will only return after all
//  the other parts of the framework are terminated. You shouldn't
//  assume the function is returned and so the standard
//  implementation is to add 'return uuiMain(argc, argv);' as the
//  last line of your int main() function in main.cpp.

int UUI uuiMain(int argc, char * argv[]);


#endif
