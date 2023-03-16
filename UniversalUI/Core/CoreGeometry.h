//  CoreGeometry.h   first written by sebhall in 2022
//
//  CoreGeometry.h defines geometry-related structs and
//  utilities.

#ifndef COREGEOMETRY_H
#define COREGEOMETRY_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

//  uSize definition
struct UUI uSize {
    float width;
    float height;
};

//  uPoint definition
struct UUI uPoint {
    float x;
    float y;
};

//  uFrame definition
struct UUI uFrame {
    float x;
    float y;
    float width;
    float height;
};

#endif /* CoreGeometry_h */
