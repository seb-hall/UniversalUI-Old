//  WinCompositor.h   first written by sebhall in March 2023
//
//  WinCompositor is a subclass of CoreCompositor, designed to
//  implement Angelo functionality in Windows

#ifndef WINCOMPOSITOR_H
#define WINCOMPOSITOR_H

#ifdef _WIN32
    #ifdef UUI_BUILD
        #define UUI __declspec(dllexport)
    #else
        #define UUI __declspec(dllimport)
    #endif
#else
    #define UUI
#endif

#include <UniversalUI/Angelo/CoreCompositor.h>

class UUI WinCompositor: public CoreCompositor { 

    unsigned int FBO;
    unsigned int VAO, VBO;
    unsigned int ShaderProgram;

public:

    //  setup compositor
    bool SetupForCompositing() override;

    //  render input buffers to output buffer
    aPixelBuffer* CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers) override;
};

#endif