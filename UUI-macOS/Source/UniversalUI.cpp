//  UniversalUI.cpp for UUI-LinuxGTK, first written by sebhall in February 2023
//  this file is the Windows specific implementation of the UniversalUI root
//  interface. There are two functions - UniversalUI() and uuiMain() - which
//  are the initialisation and main loop functions respectively.

// include required UniversalUI header files

#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Host/CoreHost.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>

#include <MacOSHost.h>

//  include standard C++ libraries

#include <stdio.h>
#include <map>
#include <string>

//	global host pointer.
MacOSHost* host;


//  Run UniversalUI. This function takes an instance of uApplication
//  but it should always be either an instance of uDesktopApplication
//  or uSimpleApplication, depending on the platform and the kind of
//  app you're designing. The framework will first check your app and
//  the installation for compatibiltiy, returning an error code (defined
//  later in this file) if there are any issues. If all is well, the
//  framework will then start the main loop and call the FinishedLaunching
//  method on your uApplication instance.
int UniversalUI(uApplication* userApp) {

	//	initialise host on stack
	host = new MacOSHost;

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

	//  assign app and host references
	host->app = userApp;
	userApp->host = host;

	//  everything started successfully so display welcome message
	printf("\n\t*** Welcome to UniversalUI D3! ***\n\n");
	
	return host->main();
}
