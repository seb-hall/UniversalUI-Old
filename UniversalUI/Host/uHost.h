//  uHost.h   first written by Seb H in March 2023
//
//  uHost is a container class designed to provide
//  communication between UniversalUI modules, and
//  to ensure correct initialisation of more complex
//  parts such as Angelo rendering.

// HOST-DEFINED

#ifndef UHOST_H
#define UHOST_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif





#endif
