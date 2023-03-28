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

class UUI CoreCompositor {
    public:

    //  setup compositor
    virtual bool SetupForCompositing();
    
    //  render input buffers to output buffer
    virtual aPixelBuffer* CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers);

};

#endif