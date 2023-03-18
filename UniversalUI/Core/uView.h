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
#include <vector>

struct aRenderOperation;

class UUI uView {
public:

    //  current frame of view, updated by parents
    uFrame frame;

    //  vector of subviews
    std::vector<uView*> subviews;

    uView(uFrame initFrame = {0.0, 0.0, 100.0, 100.0});

    //  overridable draw command
    virtual std::vector<aRenderOperation> Draw();
    
    //virtual void ClickUp();
    //virtual void ClickDown();
    //virtual void PointerEnter();
    //virtual void PointerExit();
    //virtual void PointerMoved(uPoint toPoint);
};

#endif /* uView_h */
