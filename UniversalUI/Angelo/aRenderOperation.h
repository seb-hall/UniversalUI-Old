//  aRenderOperation.h   first written by Seb H in February 2023
//
//  aRenderOperation is a structure for rendering, used for operations
//  such as drawing lines, geometry and text.

#ifndef ARENDEROPERATION_H
#define ARENDEROPERATION_H

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

// CODE THKNS  X1    Y1    X2    Y2
// LINE [2.5, 5.0, 10.0, 10.0, 10.0]

// CODE    X    Y    W    H
// IFL  [0.0, 0.0, 100.0, 50.0]

struct UUI aRenderOperation {
    unsigned short int code;
    std::vector<float> data;
};

#endif