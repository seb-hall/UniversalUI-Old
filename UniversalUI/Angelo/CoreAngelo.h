//  CoreAngelo.h   first written by Seb H in March 2023
//
//  CoreAngelo is an instance of the Angelo library. It
//  interfaces directly with instances of CoreRenderer
//  and CoreCompositor.

#ifndef COREANGELO_H
#define COREANGELO_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Angelo/CoreRenderer.h>
#include <UniversalUI/Angelo/CoreCompositor.h>
#include <UniversalUI/Angelo/aRenderCommand.h>

#include <map>

class uView;

class UUI CoreAngelo {
    public:

    std::map<uView*, aPixelBuffer*> buffers;

    CoreRenderer* renderer;
    CoreCompositor* compositor;

    //  initialise renderer
    virtual bool Init();

    //  create new pixel buffer
    virtual aPixelBuffer* GeneratePixelBuffer(uView* view);

    //  destoy pixel buffer in system and GPU memory
    virtual void DestroyPixelBuffer(aPixelBuffer* buffer);

    //  bind pixelbuffer to render target
    virtual void BindRenderTarget(aPixelBuffer* buffer);

    //  unbind current render target
    virtual void UnBindRenderTarget();

};



#endif
