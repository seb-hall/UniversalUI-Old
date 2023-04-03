//  uView.h   first written by sebhall in February 2022
//
//  uView is the base class for UUI views. All visible
//  elements will inherit from it.

#ifndef UVIEW_H
#define UVIEW_H

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
#include <UniversalUI/Angelo/CoreRenderer.h>
#include <vector>

class UUI uView {
public:

    //  will redraw if frame size changes. set needsRedraw
    //  to true if a redraw is desired at another time
    bool needsRedraw;

    //  if set to true, the view will retain it's own 
    //  dedicated pixel buffer
    bool isPersistant;

    //  current frame of view within it's superview
    uFrame frame;

    //  current frame of view within root view
    uFrame globalFrame;

    uColour backgroundColour;

    //  vector of subviews
    std::vector<uView*> subviews;

    uView(uFrame initFrame = {0.0, 0.0, 100.0, 100.0});

    //  overridable draw command
    virtual void Draw(CoreRenderer* renderer);


    //virtual void ClickUp();
    //virtual void ClickDown();
    //virtual void PointerEnter();
    //virtual void PointerExit();
    //virtual void PointerMoved(uPoint toPoint);
};

#endif /* uView_h */
