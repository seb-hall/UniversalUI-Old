//  LinuxGTKAngelo.cpp   first written by sebhall in March 2023

#include <LinuxGTKAngelo.h>
#include <epoxy/gl.h>

//  destoy pixel buffer in system and GPU memory
void LinuxGTKAngelo::DestroyPixelBuffer(aPixelBuffer* buffer) {
    glDeleteTextures(1, &buffer->id);
    delete buffer;
}

//  render view command for view 
void LinuxGTKAngelo::RenderViewCommand(uView* view, aRenderCommand command) {

}

//  render text for view
void LinuxGTKAngelo::RenderViewText(uView* view, std::string text) {

}

//  render image for view
void LinuxGTKAngelo::RenderViewImage(uView* view, std::string path) { 

}