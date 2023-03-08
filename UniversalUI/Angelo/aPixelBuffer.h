//  aPixelBuffer.h   first written by Seb H in February 2023
//
//  aPixelBuffer is a generic pixel buffer object, used to
//  represent objects such as a texture in OpenGL.

#ifndef ARENDERTARGET_H
#define ARENDERTARGET_H

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

struct UUI aPixelBuffer {
    uSize size;
    unsigned long long int id;
};

#endif