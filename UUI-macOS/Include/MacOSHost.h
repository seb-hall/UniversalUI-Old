//  MacOSHost.h   first written by sebhall in March 2023
//
//  MacOSHost is a subclass of CoreHost, designed as the
//  standard macOS host for UniversalUI applications.

#ifndef MACOSHOST_H
#define MACOSHOST_H

#define UUI

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Host/CoreHost.h>
#include <string>

class UUI MacOSHost: public CoreHost {

public:
    int main() override;
    void ShowWindow(uWindow* window) override;
    void SetTitle(uWindow* window, std::string title) override;
};
#endif
