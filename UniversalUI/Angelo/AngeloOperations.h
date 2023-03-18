//  AngeloOperations.h  first written by sebhall in March 2023
//
//  this file contains drawing functions for rendering 2D 
//  geometry with the Anglelo rendering framework.

#ifndef ANGELOOPERATIONS_H
#define ANGELOOPERATIONS_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Angelo/aRenderOperation.h>
#include <UniversalUI/Core/CoreGeometry.h>
#include <UniversalUI/Core/CoreColour.h>

//  set the drawing colour for subsequent operations
aRenderOperation UUI AngeloColour(uColour colour);

//  set the drawing weight / stroke thickness for subsequent operations
aRenderOperation UUI AngeloWeight(float weight);


//  draw a line between first and second points
aRenderOperation UUI AngeloLine(uPoint first, uPoint second);

//  draw a circle with centre and radius
aRenderOperation UUI AngeloCircle(uPoint centre, float radius);

//  draw an arc with centre and radius, starting angle and duration of angle
aRenderOperation UUI AngeloArc(uPoint centre, float radius, float startAngle, float duration);

//  draw an aligned rectangle between two points
aRenderOperation UUI AngeloRectangle(uPoint first, uPoint second);

//  draw an aligned rectangle between two points and with corner radius
aRenderOperation UUI AngeloRoundedRectangle(uPoint first, uPoint second, float cornerRadius);

#endif