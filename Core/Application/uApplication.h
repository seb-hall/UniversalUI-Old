// uApplication.h
// first written in early 2022 by Seb Hall, this version first written on 13/10/2022 also by Seb Hall
//
// UniversalUI files are intended for use with UniversalUI programs only. I accept no responsibility for performance or stability, use at your own risk
// 
// uApplication is the library class for application control, intended to be overridden by the end developer. Please only create one instance of uApplication per project.
// For more information, please see our website.

#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include <string>
#include <vector>
#include <stdio.h>

#include "../Protocol/UniversalUI-Protocol.h"
#include "../Interface/UniversalUI-Interface.h"

class uApplication {

public:

    std::vector<uWindow*> windows = { };
    
    uApplicationManagerProtocol* ApplicationManager;
    uInterfaceManagerProtocol* InterfaceManager;

    // methods to be overridden:

    virtual void ApplicationLaunched() {
        
    }

    virtual void ApplicationWillSuspend() {

    }

    virtual bool ApplicationShouldTerminate() {

        return true;
    }

    // Base methods: 

    void AddWindow(uWindow* window) {
        windows.push_back(window);
        InterfaceManager->AddWindow(window);
    }

};

#endif