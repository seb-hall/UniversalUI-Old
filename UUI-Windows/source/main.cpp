#include <uui-win.h>

struct uRect {
    float x;
    float y;
    float width;
    float height;
};

struct uPoint {
    float x;
    float y;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
void window_refresh_callback(GLFWwindow* window);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

bool isInside(uPoint point, uRect rect) {
    return (point.x > rect.x && point.x < (rect.x + rect.width)) && (point.y > rect.y && point.y < (rect.y + rect.height));
}

uRect button = {.x = 10, .y = 10, .width = 250, .height = 50};
uPoint mousePos = {.x = 0, .y = 0};
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int scrw = 800;
int scrh = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MVP2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

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
        processInput(window);

        // render
        // ------
        glClearColor(126.0f/255.0f, 204.0f/255.0f, 204.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(button.x, button.y, button.width, button.height);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void window_refresh_callback(GLFWwindow* window) {
    glClearColor(126.0f/255.0f, 204.0f/255.0f, 204.0f/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    printf("RESIZED!\n");
    glViewport(0, 0, width, height);
    scrw = static_cast<float>(width);
    scrh = static_cast<float>(height);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    //printf("CURSORPOS: %f, %f\n", xpos, ypos);
    mousePos.x = (float)xpos;
    mousePos.y = (float)ypos;
}

