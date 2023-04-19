//  MacOSAngelo.h   first written by sebhall in March 2023
//
//  MacOSAngelo is a subclass of CoreAngelo, designed to
//  implement Angelo functionality in MacOS

#ifndef MACOSANGELO_H
#define MACOSANGELO_H

#define UUI

#include <UniversalUI/Angelo/CoreAngelo.h>
#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

class UUI MacOSAngelo: public CoreAngelo {
public:
	
	//	metal device and texture descriptor
	id<MTLDevice> metalDevice;
	MTLTextureDescriptor * textureDescriptor;
	MTLRenderPassDescriptor * renderpassDescriptor;

	//  initialise renderer
	bool Init() override;

	//  create new pixel buffer
	aPixelBuffer* GeneratePixelBuffer(uView* view) override;

	//  destoy pixel buffer in system and GPU memory
	void DestroyPixelBuffer(aPixelBuffer* buffer) override;

	//  bind pixelbuffer to render target
	void BindRenderTarget(aPixelBuffer* buffer) override;

	//  unbind current render target
	void UnBindRenderTarget() override;

};


#endif
