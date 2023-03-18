//  uDesktopApplication.cpp   first written by sebhall in February 2023
//  
//  uDesktopApplication is designed partially as a base class and partially
//  with full functionality. Virtual functions that are expected to be
//  overriden are have only placeholder implementations to keep the compiler
//  happy.

#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Host/CoreHost.h>

//  init application, taking name and version number as parameters.
uDesktopApplication::uDesktopApplication(const char* initName, int initMajorVersion, int initMinorVersion) {
    name = initName;
    majorVersion = initMajorVersion;
    minorVersion = initMinorVersion;
}

//  show a uWindow object.
bool uDesktopApplication::ShowWindow(uWindow* window) {
    host->ShowWindow(window);
    return true;
}

//  set window title
void uDesktopApplication::SetWindowTitle(uWindow* window, std::string title) {
    host->SetTitle(window, title);
}

//  *** PLACEHOLDER FUNCTIONS ***

//  called when the application has finished launching - i.e only
//  once the UniversalUI environment has been initialised and the 
//  app can actually start doing stuff like making windows!
void uDesktopApplication::FinishedLaunching(int argc, char* argv[]) { }

//  called when the application has been requested to quit by the operating
//  system. use only for last-second mandatory operations.
void uDesktopApplication::WillQuit() { }

//  called when the user has requested the application to quit. e.g by pressing
//  ctrl/cmd + q. this function returns true by default and will call the willQuit
//  function when it does. if you don't want to quit the application i.e if there
//  is unsaved work, there should be some kind of visual feedback to the user 
//  that this is the case.
bool uDesktopApplication::ShouldQuit() { return true; }

//  called when a window is moved. Override for custom behaviour.
void uDesktopApplication::WindowMoved(uWindow* window, uPoint toLocation) { }

//  called when a window is resized. Override for custom behaviour.
void uDesktopApplication::WindowResized(uWindow* window, uSize toSize) { }

