//  MacOSHost.mm    first written by sebhall in March 2023
//
//	as an attempt at reducing the confusion of Objective-C
//	classes and C++ classes and their pointers, vanilla UUI
//	i.e C++ classes should be denoted by foo* with the star
//	directly after the type name as with standard UUI style.
//	Objective-C pointers should be denoted by foo * with a
//	space following the type name.

//  include UniversalUI header files
#include <MacOSHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <MacOSAngelo.h>
#include <UniversalUI/Angelo/CoreCompositor.h>
#include <MacOSRenderer.h>

#include <Cocoa/Cocoa.h>
#include <MetalKit/MetalKit.h>

#include <map>

//	reference to global host pointer
extern MacOSHost* host;

//  structure for Cocoa/UniversalUI windows
struct SystemWindowPack {
	uWindow* window;
	NSWindow * nsWindow;
	MTKView * metalView;
};


std::map<NSWindow *, SystemWindowPack*> packMap = { };
std::map<uWindow*, SystemWindowPack*> windowMap = { };
std::map<MTKView *, SystemWindowPack*> viewMap = { };

@interface AppDelegate: NSObject <NSApplicationDelegate> @end
@interface WindowDelegate : NSObject <NSWindowDelegate, MTKViewDelegate> @end

AppDelegate * appDelegate;
WindowDelegate * windowDelegate;

int MacOSHost::main() {
	
	appDelegate = [[AppDelegate alloc] init];
	windowDelegate = [[WindowDelegate alloc] init];
	
	NSApplication * nsApplication = [NSApplication sharedApplication];
	[nsApplication setDelegate:appDelegate];
	[nsApplication activateIgnoringOtherApps:true];
	[nsApplication run];
	
	return 0;
}

void MacOSHost::ShowWindow(uWindow* window) {
	
	SystemWindowPack* pack = new SystemWindowPack;
	
	NSRect frame = NSMakeRect(0, 0, window->size.width, window->size.height);
	NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
	NSWindow * nsWindow = [[NSWindow alloc] initWithContentRect: frame styleMask: styleMask backing: NSBackingStoreBuffered defer: false];
	[nsWindow setDelegate: windowDelegate];
	[nsWindow setTitle: [NSString stringWithUTF8String: window->title.c_str()]];
	[nsWindow makeKeyAndOrderFront: nil];
	
	MTKView * metalView = [[MTKView alloc] initWithFrame: frame];
	[metalView setDelegate: windowDelegate];
	[metalView setWantsLayer: true];
	//[metalView setClearColor: MTLClearColorMake(0.0, 0.0, 0.0, 1.0)];
	[nsWindow setContentView: metalView];
	
	pack->window = window;
	pack->nsWindow = nsWindow;
	pack->metalView = metalView;

	MacOSRenderer* renderer = new MacOSRenderer;
	
	pack->window->angelo = new MacOSAngelo;
	pack->window->angelo->compositor = new CoreCompositor;
	pack->window->angelo->renderer = renderer;
	
	pack->window->angelo->compositor->parent = pack->window;
	pack->window->angelo->renderer->parent = pack->window;
	
	if (!pack->window->angelo->Init()) {
		printf("ANGELO INIT ERROR\n");
		return;
	}
	
	[metalView setDevice: renderer->metalDevice];
	[metalView setColorPixelFormat: MTLPixelFormatRGBA8Unorm];
	
	packMap[nsWindow] = pack;
	windowMap[window] = pack;
	viewMap[metalView] = pack;

}

void MacOSHost::SetTitle(uWindow* window, std::string title) {
	SystemWindowPack* pack = windowMap[window];
	[pack->nsWindow setTitle:[NSString stringWithUTF8String:title.c_str()]];
	window->title = title;
}

@implementation AppDelegate

- (void) applicationDidFinishLaunching:(NSNotification * )notification {
	printf("from host: finished launching!\n");
	host->app->FinishedLaunching();
}

@end

@implementation WindowDelegate

- (void)mtkView:(MTKView * )view drawableSizeWillChange:(CGSize)size {
	SystemWindowPack* pack = viewMap[view];
	uSize newSize = {(float)size.width, (float)size.height};
	
	if (pack->window->size.width != newSize.width || pack->window->size.height != newSize.height) {
			pack->window->size = newSize;
			
			if (host->appType == desktop) {
				uDesktopApplication* app = static_cast<uDesktopApplication*>(host->app);
				app->WindowResized(pack->window, newSize);
			} else if (host->appType == simple) {
				uSimpleApplication* app = static_cast<uSimpleApplication*>(host->app);
				app->Resized(newSize);
			}
		}
}

- (void)drawInMTKView:(MTKView * )view {
	SystemWindowPack* pack = viewMap[view];
	
	id<MTLCommandQueue> queue = [view.device newCommandQueue];
	
	
	
	// The render pass descriptor references the texture into which Metal should draw
	MTLRenderPassDescriptor * renderPassDescriptor = view.currentRenderPassDescriptor;
	if (renderPassDescriptor == nil) {
		return;
	}
	
	renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionLoad;
	
	MacOSAngelo* angelo = static_cast<MacOSAngelo*>(pack->window->angelo);
	MacOSRenderer* renderer = static_cast<MacOSRenderer*>(pack->window->angelo->renderer);
	
	printf("RENDER\n");
	
	// 	encode here
	
	// 	Create a render pass and immediately end encoding, causing the drawable to be cleared
	//id<MTLRenderCommandEncoder> commandEncoder = [renderer->metalCommandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
	
	//[commandEncoder endEncoding];
	
	// 	Get the drawable that will be presented at the end of the frame

	id<MTLTexture> outputTexture = view.currentDrawable.texture;
	printf("%u\n", outputTexture.width);
	
	aPixelBuffer* buffer = new aPixelBuffer;
	buffer->id = (__bridge void*) outputTexture;
	angelo->BindRenderTarget(buffer);
	renderer->RenderBuffer(nil);
	
	delete buffer;
	
	id<MTLCommandBuffer> commandBuffer = [queue commandBuffer];
	id<MTLRenderCommandEncoder> metalCommandEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor];
	[metalCommandEncoder endEncoding];
	[commandBuffer presentDrawable: view.currentDrawable];
	[commandBuffer commit];

	
}

@end
