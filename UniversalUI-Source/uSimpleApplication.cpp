
#include <UniversalUI/Core/uSimpleApplication.h>

//  init application, taking name and version number as parameters.
uSimpleApplication::uSimpleApplication(const char* initName, int initMajorVersion, int initMinorVersion) {
    name = initName;
    majorVersion = initMajorVersion;
    minorVersion = initMinorVersion;
}

//  called when the application has finished launching - i.e only
//  once the UniversalUI environment has been initialised and the 
//  app can actually start doing stuff like making windows!
void uSimpleApplication::FinishedLaunching(int argc, char* argv[]) {

}

//  called when the application has been requested to quit by the operating
//  system. use only for last-second mandatory operations.
void uSimpleApplication::WillQuit() {

}

//  called when the application window is resized. Override for custom behaviour.
void uSimpleApplication::WindowResized(uSize toSize) {
    
}