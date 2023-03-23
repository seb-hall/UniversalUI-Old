//  MacOSHost.mm    first written by sebhall in March 2023

//  include UniversalUI header files
#include <MacOSHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <Cocoa/Cocoa.h>

#include <map>

extern MacOSHost* host;

//  structure for GTK/Cairo/OpenGL/UniversalUI windows
struct SystemWindowPack {
	uWindow* window;
	NSWindow* nsWindow;
};

std::map<NSWindow*, SystemWindowPack*> packMap = { };
std::map<uWindow*, SystemWindowPack*> windowMap = { };

// Declare a class that conforms to NSWindowDelegate
@interface WindowDelegate : NSObject <NSWindowDelegate>
@end

// Implement the class
@implementation WindowDelegate

// Define a method that will be called when the window is resized
- (void)windowDidResize:(NSNotification *)notification {
	
	// Get the window object from the notification
	NSWindow *nsWindow = [notification object];
	// Get the new size of the window
	NSSize nsSize = [nsWindow frame].size;
	uSize newSize = {float(nsSize.width), float(nsSize.height)};
	
	SystemWindowPack* pack = packMap[nsWindow];
	
	if (host->appType == desktop) {
		uDesktopApplication* app = static_cast<uDesktopApplication*>(host->app);
		app->WindowResized(pack->window, newSize);
	} else if (host->appType == simple) {
		uSimpleApplication* app = static_cast<uSimpleApplication*>(host->app);
		app->Resized(newSize);
	}
}

@end

NSApplication* nsApplication;
WindowDelegate* windowDelegate;

bool MacOSHost::TestEnvironment() {
	nsApplication = [NSApplication sharedApplication];
	[nsApplication setActivationPolicy:NSApplicationActivationPolicyRegular];
	windowDelegate = [[WindowDelegate alloc] init];
	
    return true;
}

int MacOSHost::main() {
	
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
