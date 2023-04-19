//  MacOSRenderer.h   first written by sebhall in March 2023
//
//  MacOSRenderer is a subclass of CoreRenderer, designed to
//  implement OpenGL rendering functionality in Linux.

#ifndef MACOSRENDERER_H
#define MACOSRENDERER_H

#define UUI

#include <UniversalUI/Angelo/CoreRenderer.h>
#include <Metal/Metal.h>
#include <simd/simd.h>

typedef struct {
	simd::float2 pos;
	simd::float2 normalPos;
} ShaderVertex;

class MacOSRenderer: public CoreRenderer {

	public:

	//	metal device and command queue & buffer
	id<MTLDevice> metalDevice;
	
	//	metal renderpass descriptor descritor
	MTLRenderPassDescriptor * renderpassDescriptor;
	
	id<MTLLibrary> metalLibrary;
	id<MTLCommandQueue> metalCommandQueue;
	
	
	
	id<MTLRenderPipelineState> commandPipelineState;
	MTLRenderPipelineDescriptor * commandPipelineDescriptor;
	
	id<MTLRenderPipelineState> bufferPipelineState;
	MTLRenderPipelineDescriptor * bufferPipelineDescriptor;
	
	//  compile shaders etc
	bool Init() override;

	//  render specified operations
	void RenderOperations(std::vector<aRenderOperation> operations) override;

	//  render specified text
	void RenderText(std::string text, float size, aTextLayout layout) override;

	//  render specified image
	void RenderImage(std::string path, uSize size) override;

	//  render specified buffer
	void RenderBuffer(aPixelBuffer* buffer) override;

};



#endif
