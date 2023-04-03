//  CoreCompositor.h   first written by Seb H in March 2023
//
//  CoreCompositor is a generic compositor

#ifndef CORECOMPOSITOR_H
#define CORECOMPOSITOR_H

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
#include <UniversalUI/Angelo/aPixelBuffer.h>

#include <vector>

class uView;
class uWindow;

class UUI CoreCompositor {
    public:

    //  pointer to parent window
    uWindow* parent;

    //  setup compositor
    //virtual bool Init();

    //  render view to screen
    void CompositeView(uView* view, aPixelBuffer* ontoBuffer);

    //  create pixelbuffer and render all child views to it
    aPixelBuffer* CompositeRootView(uView* rootView);

    //  render input buffers to output buffer - NOT IMPLEMENTED FOR NOW!
    aPixelBuffer* CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers);

};

#endif