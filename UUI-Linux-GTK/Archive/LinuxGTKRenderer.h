//  LinuxGTKRenderer.h   first written by sebhall in March 2023
//
//  LinuxGTKRenderer is a subclass of CoreRenderer, designed to
//  implement OpenGL rendering functionality in Linux.

#ifndef LINUXGTKRENDERER_H
#define LINUXGTKRENDERER_H

#define UUI

#include <UniversalUI/Angelo/CoreRenderer.h>

class LinuxGTKRenderer: public CoreRenderer {

    public:


    //unsigned int genericShader;
    //unsigned int textureShader;
    //unsigned int genericVertexArray;
    //unsigned int FBO;

    //  compiler shaders and prepare for rendering
    bool InitialiseRenderer() override;

    //  compile shaders and generate buffers for window
    void InitialiseRendererForWindow(uWindow* window) override;

    //  render a window, override by platform for OpenGL/Metal functionality
    void RenderWindow(uWindow* window) override;

    //  render a view, override by platform for OpenGL/Metal functionality
    void RenderView(uView* view, uWindow* window) override;
    
    //  generate a new pixel buffer in GPU memory with a given size
    aPixelBuffer NewPixelBuffer(uSize size) override;

    //  resize an existing pixel buffer
    void ResizePixelBuffer(aPixelBuffer buffer, uSize size) override;

    //  fill a buffer with a given colour - use mainly for testing purposes
    void ClearBuffer(aPixelBuffer buffer, uColour colour) override;

};



#endif
