//  LinuxGTKRenderer.h   first written by sebhall in March 2023
//
//  LinuxGTKRenderer is a subclass of CoreRenderer, designed to
//  implement OpenGL rendering functionality in Linux.

#ifndef LINUXGTKRENDERER_H
#define LINUXGTKRENDERER_H

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

class WinRenderer: public CoreRenderer {

    public:


    unsigned int genericShader;
    unsigned int textureShader;
    unsigned int genericVertexArray;
    unsigned int FBO;

    //  compiler shaders and prepare for rendering
    bool InitialiseRenderer() override;

    //  render a window, override by platform for OpenGL/Metal functionality
    void RenderWindow(uWindow* window) override;

    //  render a view, override by platform for OpenGL/Metal functionality
    void RenderView(uView* view) override;
    
    //  generate a new pixel buffer in GPU memory with a given size
    aPixelBuffer NewPixelBuffer(uSize size) override;

    //  resize an existing pixel buffer
    void ResizePixelBuffer(aPixelBuffer buffer, uSize size) override;

    //  fill a buffer with a given colour - use mainly for testing purposes
    void ClearBuffer(aPixelBuffer buffer, uColour colour) override;

};



#endif
