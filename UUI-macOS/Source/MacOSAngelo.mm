//
//  MacOSAngelo.m
//  UniversalUI
//
//  Created by Seb on 13/04/2023.
//

#include <MacOSAngelo.h>

#include <UniversalUI/Core/uView.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <Foundation/Foundation.h>
#include <Metal.h>
#include <MetalKit/MetalKit.h>

//  initialise renderer
bool MacOSAngelo::Init() {
	
	metalDevice = MTL::CreateSystemDefaultDevice();
	metalCommandQueue = metalDevice->newCommandQueue();
	
	if (!renderer->Init()) {
		return false;
	}
	
	return true;
}

//  create new pixel buffer
aPixelBuffer* MacOSAngelo::GeneratePixelBuffer(uView* view) {
	
	// 	create UUI pixelBuffer object
	aPixelBuffer* buffer = new aPixelBuffer;
	
	//	allocate metal texture descriptor
	MTL::TextureDescriptor* textureDescriptor = MTL::TextureDescriptor::alloc()->init();
	textureDescriptor->setTextureType(MTL::TextureType2D);
	textureDescriptor->setWidth((NS::UInteger) view->frame.width);
	textureDescriptor->setHeight((NS::UInteger) view->frame.height);
	//	same format as OpenGL variants of UUI
	textureDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
	//	set GPU only access
	textureDescriptor->setStorageMode(MTL::StorageModePrivate);
	textureDescriptor->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
	
	// 	assign to pixel buffer id
	buffer->id = metalDevice->newTexture(textureDescriptor);
	
	//	deallocate metal texure descriptor
	textureDescriptor->release();
	
	return buffer;
}

//  destoy pixel buffer in system and GPU memory
void MacOSAngelo::DestroyPixelBuffer(aPixelBuffer* buffer) {
	MTL::Texture* texture = static_cast<MTL::Texture*>(buffer->id);
	texture->release();
	delete buffer;
}

//  bind pixelbuffer to render target
void MacOSAngelo::BindRenderTarget(aPixelBuffer* buffer) {
}

//  unbind current render target
void MacOSAngelo::UnBindRenderTarget() {

}
