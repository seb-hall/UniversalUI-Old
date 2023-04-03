//  aRenderCommand.h   first written by Seb H in February 2023
//
//  aRenderCommand is a structure containing Angelo drawing 
//  commands and parameters. These should be generated when
//  a view's attributes change and must be unpacked into GPU
//  buffers in order to perform a render pass

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
#include <cstdint>

struct UUI aRenderCommand {

    // size of buffer for which the command was generated. If this is different to the curent size, regenerate command.
    uSize size = {100.0f, 100.0f};

    // GPU BUFFER: continuous array of command codes
    std::vector<uint32_t> codes = { };
     
    // GPU BUFFER: continuous array of parameters
    std::vector<float> parameters = { };

    // GPU BUFFER: continuous arrray of first parameter indices for each command code - first is always 0 for clear colour
    std::vector<uint32_t> indices = { };

};

#endif