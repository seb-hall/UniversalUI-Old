//  WinAngelo.cpp   first written by sebhall in March 2023

#include <WinAngelo.h>
#include <UniversalUI/Core/uView.h>
#include <glad/glad_wgl.h>

bool WinAngelo::Init() {
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    if (!renderer->Init()) {
        return false;
    }

    return true;
}

//  create new pixel buffer
aPixelBuffer* WinAngelo::GeneratePixelBuffer(uView* view) {

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    aPixelBuffer* buffer = new aPixelBuffer;
    buffer->frame = view->frame;
    buffer->size = {view->frame.width, view->frame.height};
    glGenTextures(1, &buffer->id);
    glBindTexture(GL_TEXTURE_2D, buffer->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)view->frame.width, (int)view->frame.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return buffer;

}

//  destoy pixel buffer in system and GPU memory
void WinAngelo::DestroyPixelBuffer(aPixelBuffer* buffer) {
    glDeleteTextures(1, &buffer->id);
    delete buffer;
}

//  bind pixelbuffer to render target
void WinAngelo::BindRenderTarget(aPixelBuffer* buffer) {

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindTexture(GL_TEXTURE_2D, buffer->id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->id, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("UUI-ERROR: OpenGL framebuffer error\n");
    }
    
}

//  unbind current render target
void WinAngelo::UnBindRenderTarget() { 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}