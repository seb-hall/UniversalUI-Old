#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/CoreColour.h>
#include <UniversalUI/Core/uView.h>
//#include <UniversalUI/Core/uWindowManager.h>

#include <UniversalUI/Angelo/AngeloOperations.h>

#include <stdio.h>


class App: public uDesktopApplication {
    public:

    uWindow* window1;
    uWindow* window2;

    App() : uDesktopApplication("MYAPP", 1, 0) {

    }

    void FinishedLaunching() override {
        printf("MyApp finished launching!\n");
        
        window1 = new uWindow({1000, 750}, "App - Main");
        window2 = new uWindow({250, 750}, "App - Tools");

        window1->rootView = new uView;
        window1->rootView->backgroundColour = {1.0, 1.0, 1.0, 0.0};

        window1->background = CORE_RED;

        ShowWindow(window1);
        //ShowWindow(window2);
        
    }

    void WindowResized(uWindow* window, uSize toSize) override {
        if (window == window1) {
            std::string title = "App - Main (";
            title += std::to_string(toSize.width);
            title += ", ";
            title += std::to_string(toSize.height);
            title += ")";
            SetWindowTitle(window1, title);
        }

        if (window == window2) {
            std::string title = "App - Tools (";
            title += std::to_string(toSize.width);
            title += ", ";
            title += std::to_string(toSize.height);
            title += ")";
            SetWindowTitle(window2, title);
        }
    }

};

int main(int argc, char* argv[]) {

    App* myApp = new App();
    return UniversalUI(myApp);
}