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
#include <UniversalUI/Angelo/aRenderOperation.h>

#include <string>
#include <vector>

class uWindow;

enum aTextLayout {
    left,
    centre,
    right
};

class UUI CoreRenderer {
    public:

    //  pointer to parent window
    uWindow* parent;

    //  rendering frame
    uFrame renderFrame;

    //  setup framebuffer etc
    virtual bool Init();

    //  render specified operations
    virtual void RenderOperations(std::vector<aRenderOperation> operations);

    //  render specified text
    virtual void RenderText(std::string text, float size, aTextLayout layout);

    //  render specified image
    virtual void RenderImage(std::string path, uSize size);

    //  render specified buffer
    virtual void RenderBuffer(aPixelBuffer* buffer);

};

#endif