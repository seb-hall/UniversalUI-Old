//  MacOSRenderer.mm    first written by sebhall in March 2023

#include <MacOSHost.h>
#include <MacOSRenderer.h>
#include <UniversalUI/Angelo/CoreRenderer.h>

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Angelo/aPixelBuffer.h>


//  include platform-specific libraries
#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

//  include standard C++ library functions
#include <iostream>
#include <stdio.h>
#include <cstdint>


//  compiler shaders and prepare for rendering
bool MacOSRenderer::Init() {

	metalCommandQueue = [metalDevice newCommandQueue];
	return true;
}

//  render specified operations
void MacOSRenderer::RenderOperations(std::vector<aRenderOperation> operations) {


}

//  render specified text
void MacOSRenderer::RenderText(std::string text, float size, aTextLayout layout) {

}

//  render specified image
void MacOSRenderer::RenderImage(std::string path, uSize size) {
	
}

//  render specified buffer
void MacOSRenderer::RenderBuffer(aPixelBuffer* buffer) {


}
