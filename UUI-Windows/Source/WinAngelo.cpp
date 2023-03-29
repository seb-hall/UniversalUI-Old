//  WinAngelo.cpp   first written by sebhall in March 2023

#include <WinAngelo.h>
#include <glad/glad_wgl.h>

//  destoy pixel buffer in system and GPU memory
void WinAngelo::DestroyPixelBuffer(aPixelBuffer* buffer) {
    glDeleteTextures(1, &buffer->id);
    delete buffer;
}

//  render view command for view 
void WinAngelo::RenderViewCommand(uView* view, aRenderCommand command) {

}

//  render text for view
void WinAngelo::RenderViewText(uView* view, std::string text) {

}

//  render image for view
void WinAngelo::RenderViewImage(uView* view, std::string path) { 

}