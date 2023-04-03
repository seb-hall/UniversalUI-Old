//  LinuxGTKAngelo.h   first written by sebhall in March 2023
//
//  LinuxGTKAngelo is a subclass of CoreAngelo, designed to
//  implement Angelo functionality in Linux

#ifndef LINUXGTKANGELO_H
#define LINUXGTKANGELO_H

#define UUI

#include <UniversalUI/Angelo/CoreAngelo.h>

class UUI LinuxGTKAngelo: public CoreAngelo { 
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