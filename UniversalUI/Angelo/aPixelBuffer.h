//  aPixelBuffer.h   first written by Seb H in February 2023
//
//  aPixelBuffer is a generic pixel buffer object, used for 
//  compositing and as a render target.

#ifndef APIXELBUFFER_H
#define APIXELBUFFER_H

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
#include <cstdint>

struct UUI aPixelBuffer {
    uSize size;                 //  size of allocation (float but should never be fractional)
    uFrame frame;               //  compositing frame
    unsigned int id;  //  buffer instance
};

#endif