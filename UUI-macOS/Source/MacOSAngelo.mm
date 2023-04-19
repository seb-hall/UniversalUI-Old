//
//  MacOSAngelo.m
//  UniversalUI
//
//  Created by Seb on 13/04/2023.
//

#include <MacOSAngelo.h>
#include <MacOSRenderer.h>

#include <UniversalUI/Core/uView.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <Foundation/Foundation.h>
#include <MetalKit/MetalKit.h>

//  initialise renderer
bool MacOSAngelo::Init() {
	
	//	init metalDevice
	metalDevice = MTLCreateSystemDefaultDevice();
	
	//	setup texture descriptor
	textureDescriptor = [[MTLTextureDescriptor alloc] init];
	[textureDescriptor setTextureType: MTLTextureType2D];
	[textureDescriptor setPixelFormat: MTLPixelFormatRGBA8Unorm];
	[textureDescriptor setStorageMode: MTLStorageModePrivate];
	[textureDescriptor setUsage: MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite];
	
	//	setup renderpass descriptor
	renderpassDescriptor = [[MTLRenderPassDescriptor alloc] init];
	renderpassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
	renderpassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(1.0, 0.0, 0.0, 1.0);
	renderpassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
	
	//	assign renderer metaldevice to the same one
	MacOSRenderer* macRenderer = static_cast<MacOSRenderer*>(renderer);
	macRenderer->metalDevice = metalDevice;
	macRenderer->renderpassDescriptor = renderpassDescriptor;
	
	//	init renderer
	if (!renderer->Init()) {
		return false;
	}
	
	return true;
}

//  create new pixel buffer
aPixelBuffer* MacOSAngelo::GeneratePixelBuffer(uView* view) {
	
	// 	create UUI pixelBuffer object
	aPixelBuffer* buffer = new aPixelBuffer;
	
	//	set new texture descriptor size
	[textureDescriptor setWidth: (NSUInteger) view->frame.width];
	[textureDescriptor setHeight: (NSUInteger) view->frame.height];

	// 	create resource and assign to pixel buffer id as a retained pointer (manual memory control)
	buffer->id = (__bridge_retained void*) [metalDevice newTextureWithDescriptor: textureDescriptor];
	
	return buffer;
}

//  destoy pixel buffer in system and GPU memory
void MacOSAngelo::DestroyPixelBuffer(aPixelBuffer* buffer) {
	
	//	transfer texture into ARC management and let go out of scope
	id<MTLTexture> texture = (__bridge_transfer id<MTLTexture>)buffer->id;
	delete buffer;
	
}

//  bind pixelbuffer to render target
void MacOSAngelo::BindRenderTarget(aPixelBuffer* buffer) {
	id<MTLTexture> texture = (__bridge id<MTLTexture>)buffer->id;
	renderpassDescriptor.colorAttachments[0].texture = texture;
}

//  unbind current render target
void MacOSAngelo::UnBindRenderTarget() {

}
