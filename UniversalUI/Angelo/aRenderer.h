//  aRenderer.h   first written by Seb H in March 2023
//
//  aRenderer is a generic renderer object, designed 
//  as an abstraction layer over the underlying API, 
//  be that OpenGL, Metal or possibly in the future, 
//  Vulkan. In the interest of simplicity, for now
//  aRenderer will perform most of the actual render
//  management, but may be split up in future

// HOST-DEFINED

#ifndef ARENDERER_H
#define ARENDERER_H

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

class UUI aRenderer {
    public:
    
    //  generate a new pixel buffer in GPU memory with a given size
    aPixelBuffer* NewPixelBuffer(uSize size);

    //  resize an existing pixel buffer
    void ResizePixelBuffer(aPixelBuffer* buffer, uSize size);

    //  fill a buffer with a given colour - use mainly for testing purposes
    void ClearBuffer(aPixelBuffer* buffer, uColour colour);

};

#endif