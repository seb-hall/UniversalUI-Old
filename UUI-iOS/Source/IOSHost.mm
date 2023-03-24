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

@interface ViewController : UIViewController


@end

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}


@end
    

@interface SceneDelegate : UIResponder <UIWindowSceneDelegate>

@property (strong, nonatomic) UIWindow * window;

@end


@interface SceneDelegate ()

@end

@implementation SceneDelegate


- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions {
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.windowScene = (UIWindowScene*) scene;
    
    UIViewController* viewController = [[ViewController alloc] init];
    viewController.view.backgroundColor = UIColor.orangeColor;
    self.window.rootViewController = viewController;
    [self.window makeKeyAndVisible];
}


@end

@interface AppDelegate : UIResponder <UIApplicationDelegate>


@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    host->app->FinishedLaunching();

    return YES;
}

@end


UIApplication* uiApplication;
AppDelegate* appDelegate;

int IOSHost::main() {
    
    int argc = 0;
    char* argv[] = {};
    
    printf("got to here\n");
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
    
}

