//
//  main.cpp
//  MyApp
//
//  Created by Seb on 23/03/2023.
//

#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <stdio.h>

class MyApp: public uSimpleApplication {
public:
    
    MyApp() : uSimpleApplication("MYAPP", 1, 0) {

    }
    
    void FinishedLaunching(int argc, char* argv[]) override {
        printf("yay finished launching great success\n");
    }
    
};


int main(int argc, char* argv[]) {
    
    MyApp* app = new MyApp;
    if (!UniversalUI(app)) {
        return -1;
    }
    
    return uuiMain(argc, argv);
}
