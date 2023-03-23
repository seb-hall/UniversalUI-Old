//  IOSHost.mm    first written by sebhall in March 2023

//  include UniversalUI header files
#include <IOSHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <map>
#include <UIKit/UIKit.h>

extern IOSHost* host;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
    
    // Create a red view and add it as a subview of the window
    UIView *redView = [[UIView alloc] initWithFrame:CGRectMake(100, 100, 200, 200)];
    redView.backgroundColor = [UIColor redColor];
    [self.window addSubview:redView];
    
    [self.window makeKeyAndVisible];
    return YES;
}

@end


UIApplication* uiApplication;
AppDelegate* appDelegate;

bool IOSHost::TestEnvironment() {
    uiApplication = [UIApplication sharedApplication];
    appDelegate = [[AppDelegate alloc] init];
    [uiApplication setDelegate:appDelegate];
    return true;
}

int IOSHost::main() {
    
    UIApplicationMain(nil, nil, NSStringFromClass(), <#NSString * _Nullable delegateClassName#>)
    return 0;
}

