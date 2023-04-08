//  CoreColour.h   first written by sebhall in 2022
//
//  CoreColour.h defines colour-related structs and
//  utilities.
//
//  N.B CoreColour uses the English (traditional) spelling
//  convention. An English (simplified) version using
//  CoreColor is planned at some point in the future.

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

//  uColour struct definition - the basis of all
//  colours in UniversalUI in an RGBA layout.
struct UUI uColour {
    float r;
    float g;
    float b;
    float a;
};

//  standard uColour implementations for easy colouring

#define CORE_BLACK  { 0.0, 0.0, 0.0, 1.0 }
#define CORE_RED    { 0.9, 0.2, 0.2, 1.0 }
#define CORE_GREEN  { 0.2, 0.9, 0.2, 1.0 }
#define CORE_BLUE   { 0.2, 0.2, 0.9, 1.0 }
#define CORE_ALMOST_BLACK   { 0.05, 0.05, 0.05, 1.0 }

#endif /* CoreColour_h */
