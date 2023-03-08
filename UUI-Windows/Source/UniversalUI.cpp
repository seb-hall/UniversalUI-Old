//  UniversalUI.cpp for UUI-Windows, first written by Seb H in February 2023
//  this file is the Windows specific implementation of the UniversalUI root
//  interface. There are two functions - UniversalUI() and uuiMain() - which
//  are the initialisation and event loop functions respectively.

#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>

//  include standard C++ libraries

#include <stdio.h>
#include <map>
#include <string>

//  include GLAD and GLFW libraries used for OpenGL API management and 
//  interfacing with the Win32 API respectively.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//  Initialise UniversalUI - checking for installation and user
//  aplication compatibility. If all is well, the function will
//  return true, and uuiMain should be called to start your app.
//  If the function returns false, there are errors in the host
//  platform i.e no OpenGL support or missing resources. The
//  int main() function should return at this point as UniversalUI
//  will not run correctly.

uApplication* app;

bool UniversalUI(uApplication* userApp) {

    //  initialise GLFW check for OpenGL 3.3 context availability
    //  by creating a test window and checking if it was created
    //  successfully.

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, false);

    GLFWwindow* testWindow = glfwCreateWindow(800, 600, "UUI-TESTWINDOW", NULL, NULL);

    if (!testWindow) {
        printf("UUI-CRITICAL: GLFW Window Creation Failed!\n");
        glfwTerminate();
        return false;
    }

    //  initialise GLAD as OpenGL 3.3 and GLFW init succeeded
    glfwMakeContextCurrent(testWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("UUI-CRITICAL: GLAD Initialisation Failed!\n");
        glfwTerminate();
        return false;
    }
    

    //  all window checks succeeded so destroy test window

    glfwDestroyWindow(testWindow);
    glfwWindowHint(GLFW_VISIBLE, true);

    if (dynamic_cast<uSimpleApplication*>(userApp)) {
        printf("UUI-INFO: Simple Application Created\n");
    } else if (dynamic_cast<uDesktopApplication*>(userApp)) {
        printf("UUI-INFO: Desktop Application Created\n");
    } else {
        printf("UUI-CRITICAL: Invalid Application Created! Please use subclass either uSimpleApplication or uDesktopApplication.\n");
        glfwTerminate();
        return false;
    }

    printf("\n\t*** Welcome to UniversalUI D3! ***\n\n");
    
    app = userApp;
    return true;
}

//  Run UniversalUI. This function will only return after all
//  the other parts of the framework are terminated. You shouldn't
//  assume the function is returned and so the standard
//  implementation is to add 'return uuiMain(argc, argv);' as the
//  last line of your int main() function in main.cpp.

int uuiMain(int argc, char* argv[]) {

    app->FinishedLaunching(argc, argv);

    GLFWwindow* testWindow = glfwCreateWindow(800, 600, "UUI-TESTWINDOW", NULL, NULL);

    while (true) {


        glfwWaitEvents();
    }

    glfwTerminate();
    return 0;
}
