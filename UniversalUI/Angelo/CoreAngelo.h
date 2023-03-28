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

    std::map<uView*, aPixelBuffer> buffers;

public:

    CoreRenderer* renderer;
    CoreCompositor* compositor;

    //  destoy pixel buffer in system and GPU memory
    virtual void DestroyPixelBuffer(aPixelBuffer* buffer);

    //  render view command for view 
    virtual void RenderViewCommand(uView* view, aRenderCommand command);
    
    //  render text for view
    virtual void RenderViewText(uView* view, std::string text);

    //  render image for view
    virtual void RenderViewImage(uView* view, std::string path);

};



#endif
