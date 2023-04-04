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
    unsigned int FBO;
public:

    //  initialise renderer
    bool Init() override;

    //  create new pixel buffer
    aPixelBuffer* GeneratePixelBuffer(uView* view) override;

    //  destoy pixel buffer in system and GPU memory
    void DestroyPixelBuffer(aPixelBuffer* buffer) override;

    //  bind pixelbuffer to render target
    void BindRenderTarget(aPixelBuffer* buffer) override;

    //  unbind current render target
    void UnBindRenderTarget() override;

};


#endif