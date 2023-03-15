#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uDesktopApplication.h>
//#include <UniversalUI/Core/uWindowManager.h>

#include <stdio.h>


class App: public uDesktopApplication {
    public:
    App() : uDesktopApplication("MYAPP", 1, 0) {

    }

    void FinishedLaunching(int argc, char* argv[]) override {
        printf("MyApp finished launching!\n");
        //windowManager->CreateWindow("YO", {500, 500});
         //windowManager->CreateWindow("YO 2", {500, 500});
    }

};

int main(int argc, char* argv[]) {

    App* myApp = new App();
    if (!UniversalUI(myApp)) {
        return -1;
    }

    return uuiMain(argc, argv);
}