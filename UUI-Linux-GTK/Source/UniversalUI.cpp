//  UniversalUI.cpp for UUI-LinuxGTK, first written by sebhall in February 2023
//  this file is the Windows specific implementation of the UniversalUI root
//  interface. There are two functions - UniversalUI() and uuiMain() - which
//  are the initialisation and main loop functions respectively.

// include required UniversalUI header files

#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Host/CoreHost.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>

#include <LinuxGTKHost.h>

//  include standard C++ libraries

#include <stdio.h>
#include <map>
#include <string>

// global host pointer.
LinuxGTKHost* host;

//  Initialise UniversalUI - checking for installation and user
//  aplication compatibility. If all is well, the function will
//  return true, and uuiMain should be called to start your app.
//  If the function returns false, there are errors in the host
//  platform i.e no OpenGL support or missing resources. The
//  int main() function should return at this point as UniversalUI
//  will not run correctly.
bool UniversalUI(uApplication* userApp) {

    host = new LinuxGTKHost;

    //  check format of userApp - simple, desktop or (wrongly) base uApplication
    if (dynamic_cast<uSimpleApplication*>(userApp)) {
        printf("UUI-INFO: Simple Application Created\n");
        host->appType = simple;
    } else if (dynamic_cast<uDesktopApplication*>(userApp)) {
        printf("UUI-INFO: Desktop Application Created\n");
        host->appType = desktop;
    } else {
        printf("UUI-CRITICAL: Invalid Application Created! Please use subclass either uSimpleApplication or uDesktopApplication.\n");
        host->appType = invalid;
        return false;
    }

    if (!host->TestEnvironment()) {
        return false;
    }

    printf("\n\t*** Welcome to UniversalUI D3! ***\n\n");
    
    host->app = userApp;
    userApp->host = host; 

    return true;
}

//  Run UniversalUI. This function will only return after all
//  the other parts of the framework are terminated. You shouldn't
//  assume the function is returned and so the standard
//  implementation is to add 'return uuiMain(argc, argv);' as the
//  last line of your int main() function in main.cpp.
int uuiMain(int argc, char* argv[]) {

    // call finished launching function for app.
    host->app->FinishedLaunching(argc, argv);

    return host->main();
}