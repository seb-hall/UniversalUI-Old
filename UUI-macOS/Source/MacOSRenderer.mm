//  MacOSRenderer.mm    first written by sebhall in March 2023

#include <MacOSHost.h>
#include <MacOSRenderer.h>
#include <AngeloCore.h>
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

static const ShaderVertex standardVertices[] = {
	{{-1.0f, -1.0f}, {0.0f, 0.0f}},
	{{1.0f,  -1.0f}, {1.0f, 0.0f}},
	{{1.0f,   1.0f}, {1.0f, 1.0f}},
	{{-1.0f, -1.0f}, {0.0f, 0.0f}},
	{{1.0f,   1.0f}, {1.0f, 1.0f}},
	{{-1.0f,  1.0f}, {0.0f, 1.0f}}
};


//  compiler shaders and prepare for rendering
bool MacOSRenderer::Init() {
	NSError * error = NULL;
	MTLCompileOptions * options;
	[options setFastMathEnabled: true];
	metalLibrary = [metalDevice newLibraryWithSource: [[NSString alloc] initWithCString: AngeloCore encoding: NSUTF8StringEncoding] options: options error: NULL];
	if (error) {
		printf("LIBRARY state creation error!\n ");
		printf(error.localizedDescription.cString);
	}
	
	metalCommandQueue = [metalDevice newCommandQueue];
	
	//	initialise pipeline descriptor with core functions
	commandPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	commandPipelineDescriptor.vertexFunction = [metalLibrary newFunctionWithName: @"AngeloCommandVS"];
	commandPipelineDescriptor.fragmentFunction = [metalLibrary newFunctionWithName: @"AngeloCommandFS"];
	commandPipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
	
	
	commandPipelineState = [metalDevice newRenderPipelineStateWithDescriptor: commandPipelineDescriptor error: &error];
	if (!commandPipelineState) {
		printf("COMMAND state creation error!\n");
	}
	
	bufferPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	bufferPipelineDescriptor.vertexFunction = [metalLibrary newFunctionWithName: @"AngeloBufferVS"];
	bufferPipelineDescriptor.fragmentFunction = [metalLibrary newFunctionWithName: @"AngeloBufferFS"];
	bufferPipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
	
	bufferPipelineState = [metalDevice newRenderPipelineStateWithDescriptor: bufferPipelineDescriptor error: &error];
	if (!bufferPipelineState) {
		printf("BUFFER state creation error!\n");
	}
	
	
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
	id<MTLCommandBuffer> commandBuffer = [metalCommandQueue commandBuffer];
	id<MTLRenderCommandEncoder> metalCommandEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderpassDescriptor];
	[metalCommandEncoder setLabel:  @"AngeloCoreCommand"];
	[metalCommandEncoder setRenderPipelineState: bufferPipelineState];
	[metalCommandEncoder setVertexBytes: standardVertices length: sizeof(standardVertices) atIndex: 0];
	[metalCommandEncoder drawPrimitives: MTLPrimitiveTypeTriangleStrip vertexStart: 0 vertexCount: 6];
	[metalCommandEncoder endEncoding];
	[commandBuffer commit];

}
