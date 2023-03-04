//  CoreGeometry.h   first written by Seb H in 2022
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

struct UUI uSize {
    float width;
    float height;
};

struct UUI uPoint {
    float x;
    float y;
};

#endif /* CoreGeometry_h */
