//  WinHost.cpp    first written by sebhall in March 2023

//  include UniversalUI header files
#include <WinHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>

//  include standard C++ libraries
#include <stdio.h>
#include <string>
#include <map>

//  include platform-specific libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//  reference to global host pointer
extern WinHost* host;

//  structure for GTK/Cairo/OpenGL/UniversalUI windows
struct SystemWindowPack {
    uWindow* window;
    GLFWwindow* glfwWindow;
    unsigned int VAO, VBO, pixelbuffer, framebuffer;
};

std::map<uWindow*, SystemWindowPack*> windowMap = { };

//  forward definition of callbacks
void ResizeCallback(GLFWwindow* glfwWindow, int width, int height);
void CursorCallback(GLFWwindow* glfwWindow, double x, double y);

//  takes a pointer to a windowPack and creates new GLFW & OpenGL resources
void DeployWindowPack(SystemWindowPack* pack) {

    pack->glfwWindow = glfwCreateWindow((int)pack->window->size.width, (int)pack->window->size.height, pack->window->title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(pack->glfwWindow, pack);
    glfwMakeContextCurrent(pack->glfwWindow);
    glfwSetCursorPosCallback(pack->glfwWindow, CursorCallback);
    glfwSetFramebufferSizeCallback(pack->glfwWindow, ResizeCallback);

    glClearColor(pack->window->background.r, pack->window->background.g, pack->window->background.b, pack->window->background.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(pack->glfwWindow);
}

void WinHost::ShowWindow(uWindow* window) {

    //  instantiate a new windowPack and show on screen
    SystemWindowPack* pack = new SystemWindowPack;
    windowMap[window] = pack;
    pack->window = window;
    DeployWindowPack(pack);
}

bool WinHost::TestEnvironment() {

    //  initialise GLFW check for OpenGL 3.3 context availability
    //  by creating a test window and checking if it was created
    //  successfully.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    //glfwWindowHint(GLFW_DECORATED, false);
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

    return true;
}

int WinHost::main() {

    while (true) {

        glfwWaitEvents();

    }

    glfwTerminate();
    return 0;
}

void WinHost::SetTitle(uWindow* window, std::string title) {
    glfwSetWindowTitle(windowMap[window]->glfwWindow, title.c_str());
    window->title = title;
}

void CursorCallback(GLFWwindow* glfwWindow, double x, double y) {
    //printf("CURSOR: %f %f\n", x, y);
}

void ResizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    
    SystemWindowPack* pack = static_cast<SystemWindowPack*>(glfwGetWindowUserPointer(glfwWindow));

    uSize newSize = {(float) width, (float) height};

    if (pack->window->size.width != newSize.width || pack->window->size.height != newSize.height) {
        pack->window->size = newSize;
        if (host->appType == desktop) {
            uDesktopApplication* app = static_cast<uDesktopApplication*>(host->app);
            app->WindowResized(pack->window, newSize);
        } else if (host->appType == simple) {
            uSimpleApplication* app = static_cast<uSimpleApplication*>(host->app);
            app->Resized(newSize);
        }
    }

    glfwMakeContextCurrent(glfwWindow);

    glClearColor(pack->window->background.r, pack->window->background.g, pack->window->background.b, pack->window->background.a);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(glfwWindow);
}