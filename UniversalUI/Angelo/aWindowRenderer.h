//  aWindowRenderer.h   first written by Seb H in March 2023
//
//  aWindowRenderer is a platform-specific

// HOST-DEFINED

#ifndef AWINDOWRENDERER_H
#define AWINDOWRENDERER_H

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

class UUI aWindowRenderer {
    public:
    void RenderWindow(uWindow* window);
};

#endif