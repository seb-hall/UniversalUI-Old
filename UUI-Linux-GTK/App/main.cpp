#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/CoreColour.h>
//#include <UniversalUI/Core/uWindowManager.h>

#include <stdio.h>


class App: public uDesktopApplication {
    public:

    uWindow* window1;
    uWindow* window2;

    App() : uDesktopApplication("MYAPP", 1, 0) {

    }

    void FinishedLaunching(int argc, char* argv[]) override {
        printf("MyApp finished launching!\n");
        
        window1 = new uWindow({1000, 750}, "App - Main");
        window2 = new uWindow({250, 750}, "App - tools");

        window1->background = UUI_RED;

        ShowWindow(window1);
        ShowWindow(window2);
        
    }

};

int main(int argc, char* argv[]) {

    App* myApp = new App();
    if (!UniversalUI(myApp)) {
        return -1;
    }

    return uuiMain(argc, argv);
}