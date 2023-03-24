//  MacOSRenderer.mm    first written by sebhall in March 2023

#include <MacOSHost.h>
#include <MacOSRenderer.h>
#include <UniversalUI/Angelo/CoreRenderer.h>

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Angelo/aPixelBuffer.h>


//  include platform-specific libraries
#include <Metal.h>
#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

//  include standard C++ library functions
#include <iostream>
#include <stdio.h>
#include <cstdint>


//  compiler shaders and prepare for rendering
bool MacOSRenderer::InitialiseRenderer() {

	
	return true;
}

//  render a view, override by platform for OpenGL/Metal functionality
void MacOSRenderer::RenderView(uView* view) {


}

//  render a window, override by platform for OpenGL/Metal functionality
void MacOSRenderer::RenderWindow(uWindow* window) {

}

//  generate a new pixel buffer in GPU memory with a given size
aPixelBuffer MacOSRenderer::NewPixelBuffer(uSize size) {
	aPixelBuffer newBuffer;
	newBuffer.size = size;

	return newBuffer;
}

//  resize an existing pixel buffer
void MacOSRenderer::ResizePixelBuffer(aPixelBuffer buffer, uSize size) {

}

//  fill a buffer with a given colour - use mainly for testing purposes
void MacOSRenderer::ClearBuffer(aPixelBuffer buffer, uColour colour) {

	printf("cleared buffer\n");
}
