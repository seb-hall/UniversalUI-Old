//  aRenderProcess.h   first written by Seb H in February 2023
//
//  aRenderProcess is a generic render process, analagous to a
//  render pass in Metal. I.e it should have defined textures,
//  buffers and operations.

#ifndef ARENDERPROCESS_H
#define ARENDERPROCESS_H

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
#include <UniversalUI/Angelo/aRenderCommand.h>

#include <vector>

class UUI aRenderProcess {
    public:
    std::vector<unsigned short int> codes;
    std::vector<float> parameters;
    std::vector<unsigned long long> pixelBuffers;
};

#endif