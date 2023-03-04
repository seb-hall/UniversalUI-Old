//  CoreColour.h   first written by Seb H in 2022
//
//  CoreColour.h defines colour-related structs and
//  utilities.

#ifndef CORECOLOUR_H
#define CORECOLOUR_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

struct UUI uColour {
    float r;
    float g;
    float b;
    float a;
};

#endif /* CoreColour_h */
