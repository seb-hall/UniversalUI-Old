//  MacOSHost.mm    first written by sebhall in March 2023

//  include UniversalUI header files
#include <MacOSHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

#include <map>

extern MacOSHost* host;

//  structure for GTK/Cairo/OpenGL/UniversalUI windows
struct SystemWindowPack {
	uWindow* window;
	NSWindow* nsWindow;
};

std::map<NSWindow*, SystemWindowPack*> packMap = { };
std::map<uWindow*, SystemWindowPack*> windowMap = { };

@interface AppDelegate : NSObject <NSWindowDelegate, MTKViewDelegate>

@property (nonatomic, strong) NSWindow *window;
@property (nonatomic, strong) MTKView *view;

@end

@implementation AppDelegate

- (void)setupWindowAndMTKView {
	// Create a window with a content view
	self.window = [[NSWindow alloc] initWithContentRect:NSMakeRect(100, 100, 800, 600)
											  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
												backing:NSBackingStoreBuffered
												  defer:NO];
	self.window.delegate = self;
	self.window.title = @"MTKView Example";
	
	// Create a MetalKit view as the content view
	self.view = [[MTKView alloc] initWithFrame:self.window.contentView.bounds device:MTLCreateSystemDefaultDevice()];
	self.view.delegate = self;
	self.view.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
	self.view.clearColor = MTLClearColorMake(0.0, 0.5, 1.0, 1.0);
	
	// Add the view to the window's content view
	[self.window.contentView addSubview:self.view];
	
	// Show the window
	[self.window makeKeyAndOrderFront:nil];
}

// Implement the MTKViewDelegate methods
- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
	// Handle resize events here
}

- (void)drawInMTKView:(MTKView *)view {
	// Draw the view here
}

@end

int MacOSHost::main() {
	
	AppDelegate* appDelegate = [[AppDelegate alloc] init];
	NSApplication* nsApplication = [NSApplication sharedApplication];

	[nsApplication activateIgnoringOtherApps:true];
	[nsApplication run];
	
	return 0;
}

void MacOSHost::ShowWindow(uWindow* window) {
	
	SystemWindowPack* pack = new SystemWindowPack;
	
	NSRect frame = NSMakeRect(0, 0, window->size.width, window->size.height);
	NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;
	NSWindow *nsWindow = [[NSWindow alloc] initWithContentRect:frame styleMask:styleMask backing:NSBackingStoreBuffered defer:false];
	[nsWindow setDelegate:windowDelegate];
	[nsWindow setTitle:[NSString stringWithUTF8String:window->title.c_str()]];
	[nsWindow makeKeyAndOrderFront:nil];
	
	pack->window = window;
	pack->nsWindow = nsWindow;
	packMap[nsWindow] = pack;
	windowMap[window] = pack;

}

void MacOSHost::SetTitle(uWindow* window, std::string title) {
	SystemWindowPack* pack = windowMap[window];
	[pack->nsWindow setTitle:[NSString stringWithUTF8String:title.c_str()]];
	window->title = title;
}
