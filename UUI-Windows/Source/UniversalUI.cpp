#include <UniversalUI/UniversalUI.h>

#include <stdio.h>
#include <map>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool UniversalUI(uApplication* userApp) {

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

    glfwWindowHint(GLFW_VISIBLE, false);

    printf("UUI-INFO: GLFW Window Creation Succeeded!\n");
    glfwTerminate();
    return false;
}