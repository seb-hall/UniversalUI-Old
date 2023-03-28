//  LinuxGTKAngelo.h   first written by sebhall in March 2023
//
//  LinuxGTKAngelo is a subclass of CoreAngelo, designed to
//  implement Angelo functionality in Linux

#ifndef LINUXGTKANGELO_H
#define LINUXGTKANGELO_H

#define UUI

#include <UniversalUI/Angelo/CoreAngelo.h>

class UUI LinuxGTKAngelo: public CoreAngelo { 
public:

    //  destoy pixel buffer in system and GPU memory
    void DestroyPixelBuffer(aPixelBuffer* buffer) override;

    //  render view command for view 
    void RenderViewCommand(uView* view, aRenderCommand command) override;
    
    //  render text for view
    void RenderViewText(uView* view, std::string text) override;

    //  render image for view
    void RenderViewImage(uView* view, std::string path) override;

};


#endif