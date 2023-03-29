//  WinRenderer.h   first written by sebhall in March 2023
//
//  WinRenderer is a subclass of CoreRenderer, designed to
//  implement OpenGL rendering functionality in Windows.

#ifndef WINRENDERER_H
#define WINRENDERER_H

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

class UUI WinRenderer: public CoreRenderer { 
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