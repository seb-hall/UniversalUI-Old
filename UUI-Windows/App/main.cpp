#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uApplication.h>

#include <stdio.h>

class App: public uApplication {
    public:
    App() : uApplication("MYAPP", 1, 0) {

    }

    void FinishedLaunching(int argc, char* argv[]) override {
        printf("MyApp finished launching!\n");
    }

};

int main(int argc, char* argv[]) {

    App* myApp = new App();
    if (!UniversalUI(myApp)) {
        return -1;
    }

    return uuiMain(argc, argv);
}