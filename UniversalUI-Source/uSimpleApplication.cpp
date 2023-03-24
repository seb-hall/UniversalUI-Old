//  uSimpleApplication.cpp   first written by sebhall in February 2023
//  
//  uSimpleApplication is designed partially as a base class and partially
//  with full functionality. Virtual functions that are expected to be
//  overriden are have only placeholder implementations to keep the compiler
//  happy.

#include <UniversalUI/Core/uSimpleApplication.h>

//  init application, taking name and version number as parameters.
uSimpleApplication::uSimpleApplication(const char* initName, int initMajorVersion, int initMinorVersion) {
    name = initName;
    majorVersion = initMajorVersion;
    minorVersion = initMinorVersion;
}

//  *** PLACEHOLDER FUNCTIONS ***

//  called when the application has finished launching - i.e only
//  once the UniversalUI environment has been initialised and the 
//  app can actually start doing stuff like making windows!
void uSimpleApplication::FinishedLaunching() { }

//  called when the application has been requested to quit by the operating
//  system. use only for last-second mandatory operations.
void uSimpleApplication::WillQuit() { }

//  called when the application window is resized. Override for custom behaviour.
void uSimpleApplication::Resized(uSize toSize) { }
