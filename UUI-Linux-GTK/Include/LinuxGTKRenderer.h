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
    unsigned int CommandShader;
    unsigned int BufferShader;
    unsigned int VAO;

public:

    //  compile shaders etc
    bool Init() override;

    //  render specified operations
    void RenderOperations(std::vector<aRenderOperation> operations) override;

    //  render specified text
    void RenderText(std::string text, float size) override;

    //  render specified image
    void RenderImage(std::string path, uSize size) override;

    //  render specified buffer
    void RenderBuffer(aPixelBuffer* buffer) override;

};


#endif