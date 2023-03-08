
#include <UniversalUI/Core/uApplication.h>

//  init application, taking name and version number as parameters.
uApplication::uApplication(const char* initName, int initMajorVersion, int initMinorVersion) {
    name = initName;
    majorVersion = initMajorVersion;
    minorVersion = initMinorVersion;
}

//  called when the application has finished launching - i.e only
//  once the UniversalUI environment has been initialised and the 
//  app can actually start doing stuff like making windows!
void uApplication::FinishedLaunching(int argc, char* argv[]) {

}

//  called when the application has been requested to quit by the operating
//  system. use only for last-second mandatory operations.
void uApplication::WillQuit() {

}

//  called when the user has requested the application to quit. e.g by pressing
//  ctrl/cmd + q. this function returns true by default and will call the willQuit
//  function when it does. if you don't want to quit the application i.e if there
//  is unsaved work, there should be some kind of visual feedback to the user 
//  that this is the case.
bool uApplication::ShouldQuit() {
    return true;
}