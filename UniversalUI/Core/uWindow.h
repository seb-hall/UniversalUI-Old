//  uWindow.h   first written by Seb H in Early 2022
//
//  uWindow is the base class for any UUI window.

#ifndef UWINDOW_H
#define UWINDOW_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Core/CoreGeometry.h>
#include <UniversalUI/Core/uView.h>

class uWindowController;

class UUI uWindow {
public:
    uSize size;
    uView* view;
    uWindowController* controller;
    uWindow();
    virtual void Loaded(uSize withSize);
    virtual void Resized(uSize toSize);
};

#endif