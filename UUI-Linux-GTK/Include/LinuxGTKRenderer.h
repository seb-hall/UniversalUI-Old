//  LinuxGTKRenderer.h   first written by sebhall in March 2023
//
//  LinuxGTKRenderer is a subclass of CoreRenderer, designed to
//  implement OpenGL rendering functionality in Linux.

#ifndef LINUXGTKRENDERER_H
#define LINUXGTKRENDERER_H

#define UUI

#include <UniversalUI/Angelo/CoreRenderer.h>
#include <vector>
#include <epoxy/gl.h>

class UUI LinuxGTKRenderer: public CoreRenderer { 
    unsigned int FBO;
public:

    //  test shader compilation etc
    bool TestEnvironment() override;

    //  setup framebuffer etc
    bool SetupForRendering() override;  

    //  render specified command
    aPixelBuffer* RenderCommand(aRenderCommand command) override;

    //  render specified text
    aPixelBuffer* RenderText(std::string text, float size) override;

    //  render specified image
    aPixelBuffer* RenderImage(std::string path) override;

};


#endif