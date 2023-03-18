//  aRenderCommand.h   first written by Seb H in February 2023
//
//  aRenderCommand is a generic render process, analagous to a
//  render pass in Metal. I.e it should have defined textures,
//  buffers and operations.

#ifndef ARENDERCOMMAND_H
#define ARENDERCOMMAND_H

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
#include <UniversalUI/Angelo/aRenderOperation.h>

#include <vector>

struct UUI aRenderCommand {

    // GPU BUFFER: continuous array of command codes - first is always the index of the final command code
    std::vector<unsigned short int> codes = { 0, 0 };
     
    // GPU BUFFER: continuous array of parameters - first is always the 4 clear colour components
    std::vector<float> parameters = { 0.0f, 0.0f, 0.0f, 0.0f };

    // GPU BUFFER: continuous arrray of first parameter indices for each command code - first is always 0 for clear colour
    std::vector<unsigned int> indices = { 0 };

    // continuous array of pixelBuffer indices - not sent to GPU but used to assign the 8 POSSIBLE INPUT TEXTURES!
    std::vector<unsigned long long> pixelBuffers = { };
};

#endif