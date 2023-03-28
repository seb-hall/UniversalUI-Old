//  CoreRenderer.h   first written by Seb H in March 2023
//
//  CoreRenderer is a generic renderer object, designed 
//  to render 2D vector geometry to pixel buffers

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

#include <string>

class UUI CoreRenderer {
    public:

    //  test shader compilation etc
    virtual bool TestEnvironment();

    //  setup framebuffer etc
    virtual bool SetupForRendering();

    //  render specified command
    virtual aPixelBuffer* RenderCommand(aRenderCommand command);

    //  render specified text
    virtual aPixelBuffer* RenderText(std::string text, float size);

    //  render specified image
    virtual aPixelBuffer* RenderImage(std::string path);

};

#endif