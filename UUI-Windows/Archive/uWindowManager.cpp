#include <UniversalUI/Core/uWindowManager.h>
#include <UniversalUI/Angelo/aRenderer.h>
#include <UniversalUI/Host/uHost.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void ResizeCallback(GLFWwindow* glfwWindow, int width, int height);
void CursorCallback(GLFWwindow* glfwWindow, double x, double y);

uWindow* uWindowManager::CreateWindow(std::string title, uSize size, int options) {
    GLFWwindow* glfwWindow = glfwCreateWindow((int) size.width, (int) size.height, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(glfwWindow);
    glfwSetCursorPosCallback(glfwWindow, CursorCallback);
    glfwSetFramebufferSizeCallback(glfwWindow, ResizeCallback);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(glfwWindow);

    uWindow* window = new uWindow;
    window->size = size;
    window->title = title;

    windowBuffers[window] = host->renderer->NewPixelBuffer(size);
    ptrWindows[glfwWindow] = window;
    return window;

}
uWindow* uWindowManager::CreateWindowFromFile(std::string filePath) {

}
void uWindowManager::WindowMoved(uWindow* window, uPoint toLocation) {

}
void uWindowManager::WindowResized(uWindow* window, uSize toSize) {

}

void CursorCallback(GLFWwindow* glfwWindow, double x, double y) {
    //printf("CURSOR: %f %f\n", x, y);
}

void ResizeCallback(GLFWwindow* glfwWindow, int width, int height) {
     glfwMakeContextCurrent(glfwWindow);
    if (width < 500) {
        glClearColor(1.0, 0.0, 0.0, 1.0);
    } else {
        glClearColor(1.0, 1.0, 0.0, 1.0);
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(glfwWindow);
}