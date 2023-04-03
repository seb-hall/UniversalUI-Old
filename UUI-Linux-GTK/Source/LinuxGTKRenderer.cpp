//  LinuxGTKRenderer.cpp    first written by sebhall in March 2023
//
//  LinuxGTKRenderer is a subclass of CoreRenderer, designed to
//  implement OpenGL rendering functionality in Linux.

#include <LinuxGTKRenderer.h>
#include <stdio.h>

//  compile shaders etc
bool LinuxGTKRenderer::Init() {
    return true;
}

//  render specified operations
void LinuxGTKRenderer::RenderOperations(std::vector<aRenderOperation> operations) { 
    printf("ANGELO-RENDERER: Render Operations\n");
}

//  render specified text
void LinuxGTKRenderer::RenderText(std::string text, float size) { 
    printf("ANGELO-RENDERER: Render Text\n");
}

//  render specified image
void LinuxGTKRenderer::RenderImage(std::string path, uSize size) { 
    printf("ANGELO-RENDERER: Render Image\n");
}

//  render specified buffer
void LinuxGTKRenderer::RenderBuffer(aPixelBuffer* buffer) { 
    printf("ANGELO-RENDERER: Render Buffer\n");
}
