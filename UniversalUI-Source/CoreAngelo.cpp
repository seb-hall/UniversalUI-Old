//  CoreAngelo.cpp   first written by Seb H in March 2023

#include <UniversalUI/Angelo/CoreAngelo.h>

//  initialise renderer
bool CoreAngelo::Init() { return true; }

//  create new pixel buffer
aPixelBuffer* CoreAngelo::GeneratePixelBuffer(uView* view) { return new aPixelBuffer; }

//  destoy pixel buffer in system and GPU memory
void CoreAngelo::DestroyPixelBuffer(aPixelBuffer* buffer) { }

//  bind pixelbuffer to render target
void CoreAngelo::BindRenderTarget(aPixelBuffer* buffer) { }

//  unbind current render target
void CoreAngelo::UnBindRenderTarget() { }