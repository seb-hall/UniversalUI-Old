
#include <UniversalUI/Core/uApplication.h>

//  called when the application has finished launching - i.e only
//  once the UniversalUI environment has been initialised and the 
//  app can actually start doing stuff like making windows!
void uApplication::FinishedLaunching(int argc, char* argv[]) { }

//  called when the application has been requested to quit by the operating
//  system. use only for last-second mandatory operations.
void uApplication::WillQuit() { }