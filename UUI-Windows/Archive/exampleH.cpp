#include "exampleH.h"

#include <UUI-Windows.h>

void framebufferCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(50.0f/255.0f, 204.0f/255.0f, 204.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    printf("SIZE %d %d \n", width, height);
}

void damageCallback(GLFWwindow* window) {
    glClearColor(50.0f/255.0f, 204.0f/255.0f, 204.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

int exampleH()
{
    // glfw: initialize and configure
    // ------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "MVP2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, framebufferCallback);
    glfwSetWindowRefreshCallback(window, damageCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----

        // render
        // ------
        glClearColor(126.0f/255.0f, 204.0f/255.0f, 204.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
