//  LinuxGTKCompositor.h   first written by sebhall in March 2023
//
//  LinuxGTKCompositor is a subclass of CoreCompositor, designed to
//  implement Angelo functionality in Linux

#ifndef LINUXGTKCOMPOSITOR_H
#define LINUXGTKCOMPOSITOR_H

#define UUI

#include <UniversalUI/Angelo/CoreCompositor.h>

class UUI LinuxGTKCompositor: public CoreCompositor { 

    unsigned int FBO;
    unsigned int VAO, VBO;
    unsigned int ShaderProgram;

public:

    //  setup compositor
    bool SetupForCompositing() override;

    //  render input buffers to output buffer
    aPixelBuffer* CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers) override;

    //  render view to screen
    void CompositeView(uView* view) override;
};

#endif