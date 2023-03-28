//  CoreRenderer.h   first written by Seb H in March 2023
//
//  CoreRenderer is a generic renderer object, designed 
//  as an abstraction layer over the underlying API, be 
//  that OpenGL, Metal or possibly in the future, Vulkan. 
//  In the interest of simplicity, for now aRenderer will 
//  perform most of the actual render management, but may 
//  be split up in future

#ifndef CORERENDERER_H
#define CORERENDERER_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Core/CoreGeometry.h>
#include <UniversalUI/Core/CoreColour.h>
#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Angelo/aRenderCommand.h>

#include <map>

class uWindow;
class uView;

class UUI CoreRenderer {
    public:

    std::map<uView*, aPixelBuffer> buffers;
    std::map<uView*, aRenderCommand> commands;

    //  compiler shaders and prepare for rendering
    virtual bool InitialiseRenderer();

    //  initialise window resources and fetch render commands
    void SetupWindowForRendering(uWindow* window);

    // setup view resources
    void SetupViewForRendering(uView* view);

    //  compile shaders and generate buffers for window
    virtual void InitialiseRendererForWindow(uWindow* window);

    //  render a window, override by platform for OpenGL/Metal functionality
    virtual void RenderWindow(uWindow* window);
    
    //  render a view, override by platform for OpenGL/Metal functionality
    virtual void RenderView(uView* view, uWindow* parentWindow);

    //  generate a new pixel buffer in GPU memory with a given size
    virtual aPixelBuffer NewPixelBuffer(uSize size);

    //  resize an existing pixel buffer
    virtual void ResizePixelBuffer(aPixelBuffer buffer, uSize size);

    //  fill a buffer with a given colour - use mainly for testing purposes
    virtual void ClearBuffer(aPixelBuffer buffer, uColour colour);

};

#endif