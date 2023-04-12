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
    unsigned int CommandShader;
    unsigned int BufferShader;
    unsigned int VAO, VBO;
public:

    //  compile shaders etc
    bool Init() override;

    //  render specified operations
    void RenderOperations(std::vector<aRenderOperation> operations) override;

    //  render specified text
    void RenderText(std::string text, float size, aTextLayout layout) override;

    //  render specified image
    void RenderImage(std::string path, uSize size) override;

    //  render specified buffer
    void RenderBuffer(aPixelBuffer* buffer) override;

};


#endif