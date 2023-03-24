//  IOSHost.h   first written by sebhall in March 2023
//
//  IOSHost is a subclass of CoreHost, designed as the
//  standard iOS host for UniversalUI applications.

#ifndef IOSHOST_H
#define IOSHOST_H

#define UUI

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Host/CoreHost.h>
#include <string>

class UUI IOSHost: public CoreHost {

public:
    int main() override;
};

#endif
