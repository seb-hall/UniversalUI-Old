//  uWindow.h   first created by Seb Hall in early 2022, this version in Feb 2023
//
//  uWindow is the top-level window class used in the UniversalUI framework

#ifndef UWINDOW_H
#define UWINDOW_H

#include <string>

#include <UUI/geometry.h>
#include <UUI/colour.h>

class uWindow {
public:
    uSize size;
    uColour background;
    std::string title;
    virtual void Resized(uSize toSize) { }
};

#endif