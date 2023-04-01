//  uWindow.h   first written by sebhall in Early 2022
//
//  uWindow is the base class for all UUI windows. Each
//  instance of uWindow contains it's own resources for
//  rendering and interaction etc.

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
#include <UniversalUI/Core/CoreColour.h>
#include <UniversalUI/Core/uView.h>


class CoreAngelo;

#include <string>

class UUI uWindow {
public:

    //  angelo instance for window
    CoreAngelo* angelo;

    //  root view
    uView* rootView;

    //  current size of window, updated constantly by host
    uSize size;

    //  title of window
    std::string title;

    //  background colour
    uColour background;

    //  constructor
    uWindow(uSize initSize = {800.0, 600.0}, std::string initTitle = "uWindow");

    //  main update function - called when window size changed or other rendering required
    void Update();
};

#endif