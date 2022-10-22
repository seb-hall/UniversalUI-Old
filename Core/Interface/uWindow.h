// uWindow.h (cross-platform)
// first written in early 2022 by Seb Hall, this version first written on 13/10/2022 also by Seb Hall
//
// UniversalUI files are intended for use with UniversalUI programs only. 
// I accept no responsibility for performance or stability, use at your own risk.
// 
// uWindow is a library class for app windows, intended to be overridden by the user.
// For more information, please see our website.

#ifndef UWINDOW_H
#define UWINDOW_H

#include <string>
#include "../Maths/UniversalUI-Geometry.h"

class uWindow {

    public:

    uSize size = {.width = 100, .height = 100};
    uSize minimumSize = {.width = 100, .height = 100};
    uSize initialSize = {.width = 100, .height = 100};
    uSize maximumSize = {.width = 100, .height = 100};
    std::string title = "title";

    uWindow(float width, float height, std::string initTitle) {
        size = {.width = width, .height = height};
        title = initTitle;
    }

    virtual void WindowResized() {
        
    }
};

#endif
