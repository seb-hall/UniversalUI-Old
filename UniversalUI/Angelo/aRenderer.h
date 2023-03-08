//  aRenderer.h   first written by Seb H in March 2023
//
//  aRenderer is a generic renderer object, 

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

#include <UniversalUI/Core/uWindow.h>

class UUI aRenderer {
    public:
    
};

#endif