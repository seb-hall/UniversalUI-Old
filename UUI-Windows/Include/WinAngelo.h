//  WinAngelo.h   first written by sebhall in March 2023
//
//  WinAngelo is a subclass of CoreAngelo, designed to
//  implement Angelo functionality in Windows

#ifndef WINANGELO_H
#define WINANGELO_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Angelo/CoreAngelo.h>

class UUI WinAngelo: public CoreAngelo { 
public:

    //  destoy pixel buffer in system and GPU memory
    void DestroyPixelBuffer(aPixelBuffer* buffer) override;

    //  render view command for view 
    void RenderViewCommand(uView* view, aRenderCommand command) override;
    
    //  render text for view
    void RenderViewText(uView* view, std::string text) override;

    //  render image for view
    void RenderViewImage(uView* view, std::string path) override;

};


#endif