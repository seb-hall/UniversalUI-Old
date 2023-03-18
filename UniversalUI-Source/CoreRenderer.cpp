//  CoreRenderer.cpp   first written by Seb H in March 2023

#include <UniversalUI/Angelo/CoreRenderer.h>

//  initialise window resources and fetch render commands
void CoreRenderer::SetupWindowForRendering(uWindow* window) {
    aRenderCommand* renderCommand = new aRenderCommand;
    commands[window] = renderCommand;
}

//  *** PLACEHOLDER FUNCTIONS ***

//  render a window, override by platform for OpenGL/Metal functionality
void CoreRenderer::RenderWindow(uWindow* window) { }

//  generate a new pixel buffer in GPU memory with a given size
aPixelBuffer* CoreRenderer::NewPixelBuffer(uSize size) { return new aPixelBuffer; }

//  resize an existing pixel buffer
void CoreRenderer::ResizePixelBuffer(aPixelBuffer* buffer, uSize size) { }

//  fill a buffer with a given colour - use mainly for testing purposes
void CoreRenderer::ClearBuffer(aPixelBuffer* buffer, uColour colour) { }
