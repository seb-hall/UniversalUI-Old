//  WinRenderer.cpp   first written by sebhall in March 2023

#include <WinRenderer.h>

#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <glad/glad_wgl.h>
#include <stdio.h>

//  test shader compilation etc
bool WinRenderer::TestEnvironment() {
    return true;
}

bool WinRenderer::SetupForRendering() {
    glGenFramebuffers(1, &FBO);
    return true;
}

//  render specified command
aPixelBuffer* WinRenderer::RenderCommand(aRenderCommand command) {
    return new aPixelBuffer;
}

//  render specified text
aPixelBuffer* WinRenderer::RenderText(std::string text, float size) {

    return new aPixelBuffer;
}

//  render specified image
aPixelBuffer* WinRenderer::RenderImage(std::string path) {

    return new aPixelBuffer;
}
