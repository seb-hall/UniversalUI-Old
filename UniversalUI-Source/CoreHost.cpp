//  CoreHost.cpp    first written by sebhall in late 2022
//  
//  CoreHost is designed purely as a base class and is never
//  used directly. As such, this file contains only basic
//  implementations of functions to keep the compiler happy.

#include <UniversalUI/Host/CoreHost.h>

bool CoreHost::TestEnvironment() { return true; }

int CoreHost::main() { return 0; }

void CoreHost::ShowWindow(uWindow* window) { }

void CoreHost::SetTitle(uWindow* window, std::string title) { }