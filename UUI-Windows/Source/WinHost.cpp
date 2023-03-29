//  WinHost.cpp    first written by sebhall in March 2023

//  include UniversalUI header files
#include <WinHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>

#include <WinRenderer.h>
#include <WinAngelo.h>
#include <WinCompositor.h>

//  include standard C++ libraries
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

//  include platform-specific libraries
#include <windows.h>
#include <glad/glad_wgl.h>

#define HWND_CLASS "UniversalUI-Window"

//  reference to global host pointer
extern WinHost* host;

// Define the attributes for the desired OpenGL version
int attributes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0 // End of attributes list
};

//  structure for GTK/Cairo/OpenGL/UniversalUI windows
struct SystemWindowPack {
    uWindow* window;
    HWND systemWindow;
    HINSTANCE instance;
    HDC deviceContext;
    HGLRC renderContext;
    unsigned int VAO, VBO, textureShader, framebuffer;
};

std::map<uWindow*, SystemWindowPack*> windowMap = { };
std::map<HWND, SystemWindowPack*> systemWindowMap = { };

//  forward definitions of functions
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool RegisterWindowClass(HINSTANCE &instance);
bool CreateWindowAndDC(HINSTANCE &instance, HWND &hWnd, HDC &hDC, uWindow* window);
bool RunGLTests(HDC &hDC, HGLRC &hRC);
bool CreateOpenGLContext(HDC &hDC, HGLRC &hRC);
void DeleteOpenGLContext(HWND &hWnd, HDC &hDC, HGLRC &hRC);

void DeployWindowPack(SystemWindowPack* pack) {
    pack->instance = GetModuleHandle(NULL);
    CreateWindowAndDC(pack->instance, pack->systemWindow, pack->deviceContext, pack->window);
    CreateOpenGLContext(pack->deviceContext, pack->renderContext);
    systemWindowMap[pack->systemWindow] = pack;

    // Create a vertex array object and a vertex buffer object for the quad
    glGenVertexArrays(1, &pack->VAO);
    glGenBuffers(1, &pack->VBO);
    glBindVertexArray(pack->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pack->VBO);

    // Define the quad vertices and texture coordinates
    float quad[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };
    // Upload the quad data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    // Enable the vertex attribute pointers for position and texture coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Create and compile a shader program for rendering the textures
    pack->textureShader = glCreateProgram();
    
    // Define the vertex shader source code
    const char* vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec2 aPos;\n"
                                     "layout (location = 1) in vec2 aTexCoord;\n"
                                     "out vec2 TexCoord;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x,aPos.y,0, 1.0);\n"
                                     "   TexCoord = aTexCoord;\n"
                                     "}\n";
    
    // Create and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    
    glCompileShader(vertexShader);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Define the fragment shader source code
    const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "uniform sampler2D texture1;\n"
                                   "void main()\n"
                                   "{\n" 
                                    //"   if (TexCoord.x < 0.01 || TexCoord.x > 0.99 || TexCoord.y < 0.01 || TexCoord.y > 0.99) {FragColor = vec4(1.0, 0.0, 0.0, 1.0);} else {"
                                   "   FragColor = texture(texture1, TexCoord); \n"
                                   "}\n";

    // Create and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link the vertex and fragment shaders to the shader program
    glAttachShader(pack->textureShader, vertexShader);
    glAttachShader(pack->textureShader, fragmentShader);
    glLinkProgram(pack->textureShader);

    // Check for linking errors
    glGetProgramiv(pack->textureShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pack->textureShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the vertex and fragment shaders as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    ShowWindow(pack->systemWindow, SW_SHOW);
}

void WinHost::ShowWindow(uWindow* window) {

    //  instantiate a new windowPack and show on screen
    SystemWindowPack* pack = new SystemWindowPack;
    windowMap[window] = pack;
    pack->window = window;
    DeployWindowPack(pack);
    //host->renderer->SetupWindowForRendering(window);

}

bool WinHost::TestEnvironment() {

    HINSTANCE instance = GetModuleHandle(NULL);

    HWND windowHandle = NULL;
    HDC deviceContextHandle = NULL;
    HGLRC renderContextHandle = NULL;

    if (!RegisterWindowClass(instance)) {
        return false;
    }

    if (!CreateWindowAndDC(instance, windowHandle, deviceContextHandle, new uWindow)) {
        return false;
    }

    if (!RunGLTests(deviceContextHandle, renderContextHandle)) {
        return false;
    }

    DeleteOpenGLContext(windowHandle, deviceContextHandle, renderContextHandle);

    return true;

}

int WinHost::main() {

    MSG msg;
    bool shouldQuit = false;

    // Main loop
    while (!shouldQuit)
    {
        if (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            if (msg.message == WM_QUIT) {
                shouldQuit = true;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

void WinHost::SetTitle(uWindow* window, std::string title) {
    //glfwSetWindowTitle(windowMap[window]->glfwWindow, title.c_str());
    SetWindowTextA(windowMap[window]->systemWindow, title.c_str());
    window->title = title;
}

// A function to delete the OpenGL context and release the device context
void DeleteOpenGLContext(HWND &hWnd, HDC &hDC, HGLRC &hRC)
{
    // Make the context not current
    wglMakeCurrent(NULL, NULL);

    // Delete the context
    if (hRC)
    {
        wglDeleteContext(hRC);
        hRC = NULL;
    }

    // Release the device context

    if (hDC)
    {
        ReleaseDC(hWnd, hDC);
        hDC = NULL;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    
    switch (uMsg)
    {
        case WM_DESTROY:
            exit(0);
            break;
        case WM_SIZE:
            {
                SystemWindowPack* pack = systemWindowMap[hwnd];
                    int width = LOWORD(lParam);
                    int height = HIWORD(lParam);
                uSize newSize = {(float) width, (float) height};

                if (systemWindowMap[hwnd]->window->size.width != newSize.width || systemWindowMap[hwnd]->window->size.height != newSize.height) {
                    systemWindowMap[hwnd]->window->size = newSize;
                    if (host->appType == desktop) {
                        uDesktopApplication* app = static_cast<uDesktopApplication*>(host->app);
                        app->WindowResized(systemWindowMap[hwnd]->window, newSize);
                    } else if (host->appType == simple) {
                        uSimpleApplication* app = static_cast<uSimpleApplication*>(host->app);
                        app->Resized(newSize);
                    }
                }
            
                wglMakeCurrent(systemWindowMap[hwnd]->deviceContext, systemWindowMap[hwnd]->renderContext);
                glViewport(0,0, wParam, lParam);
                glClearColor(systemWindowMap[hwnd]->window->background.r, systemWindowMap[hwnd]->window->background.g, systemWindowMap[hwnd]->window->background.b, systemWindowMap[hwnd]->window->background.a);
                glClear(GL_COLOR_BUFFER_BIT);  
                // Use the shader program
                glUseProgram(pack->textureShader);

                aPixelBuffer* buffer = host->angelo->renderer->RenderText();


                glBindTexture(GL_TEXTURE_2D, buffer->id);
                // Draw the quad with the input texture
                glDrawArrays(GL_TRIANGLES, 0, 6);

               // host->renderer->RenderWindow(systemWindowMap[hwnd]->window);
                // Swap the front and back buffers
                SwapBuffers(systemWindowMap[hwnd]->deviceContext);

                // Update the window
                UpdateWindow(hwnd);
                break;
            } 
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


bool CreateOpenGLContext(HDC &hDC, HGLRC &hRC) {
    printf("creating openGL context!");

    HGLRC tempContext = wglCreateContext(hDC);

    // Check if the temporary context was created successfully
    if (!tempContext)
    {
        //std::cerr << "Failed to create temporary context\n";
        return false;
    }

     printf("temporary created!\n");

    // Make the temporary context current
    if (!wglMakeCurrent(hDC, tempContext))
    {
        //std::cerr << "Failed to make temporary context current\n";
        return false;
    }

    printf("temporary created!\n");

    // Check if the wglCreateContextAttribsARB function is available
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB)
    {
        //std::cerr << "wglCreateContextAttribsARB is not available\n";
        return false;
    }

    hRC = wglCreateContextAttribsARB(hDC, 0, attributes);

    // Check if the final context was created successfully
    if (!hRC)
    {
        //std::cerr << "Failed to create OpenGL context\n";
        return false;
    }

    printf("full created!\n");

    // Delete the temporary context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);

    // Make the final context current
    if (!wglMakeCurrent(hDC, hRC))

    {
        //std::cerr << "Failed to make final context current\n";
        return false;
    }

    // Return true to indicate success
    return true;
}

bool RunGLTests(HDC &hDC, HGLRC &hRC) {

    // Create a temporary OpenGL context with a default version
    HGLRC tempContext = wglCreateContext(hDC);

    // Check if the temporary context was created successfully
    if (!tempContext)
    {
        //std::cerr << "Failed to create temporary context\n";
        return false;
    }

    // Make the temporary context current
    if (!wglMakeCurrent(hDC, tempContext))
    {
        //std::cerr << "Failed to make temporary context current\n";
        return false;
    }

    // Load GLAD to get access to modern OpenGL functions
    if (!gladLoadGL())
    {
        //std::cerr << "Failed to load GLAD\n";
        return false;
    }

    // Check if the wglCreateContextAttribsARB function is available
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB)
    {
        //std::cerr << "wglCreateContextAttribsARB is not available\n";
        return false;
    }

    // Create the final OpenGL context with the specified attributes
    hRC = wglCreateContextAttribsARB(hDC, 0, attributes);

    // Check if the final context was created successfully
    if (!hRC)
    {
        //std::cerr << "Failed to create OpenGL 3.3 context\n";
        return false;
    }

    // Delete the temporary context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);

    // Make the final context current
    if (!wglMakeCurrent(hDC, hRC))

    {
        //std::cerr << "Failed to make final context current\n";
        return false;
    }

    // Return true to indicate success
    return true;

}

bool RegisterWindowClass(HINSTANCE &instance) {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = HWND_CLASS;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        //std::cerr << "Failed to register window class\n";
        return false;
    }

    return true;
}

bool CreateWindowAndDC(HINSTANCE &instance, HWND &hWnd, HDC &hDC, uWindow* window) {

    RECT wr = {0, 0, (int) window->size.width, (int) window->size.height};    // set the size, but not the position
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        HWND_CLASS,
        window->title.c_str(),
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,    // width of the window
        wr.bottom - wr.top, 
        NULL,
        NULL,
        instance,
        NULL);

    // Check if the window was created successfully
    if (!hWnd)
    {
        //std::cerr << "Failed to create window\n";
        return false;
    }

        // Show and update the window
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);

    // Get the device context from the window handle
    hDC = GetDC(hWnd);

    // Check if the device context was obtained successfully
    if (!hDC)
    {
        //std::cerr << "Failed to get device context\n";
        return false;
    }

     // Define a pixel format descriptor with some default parameters
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Choose a pixel format that matches the descriptor
    int pixelFormat = ChoosePixelFormat(hDC, &pfd);

    // Check if a pixel format was found
    if (!pixelFormat)
    {
        //std::cerr << "Failed to choose pixel format\n";
        return false;

    }

    // Set the pixel format for the device context
    if (!SetPixelFormat(hDC, pixelFormat, &pfd))
    {
        //std::cerr << "Failed to set pixel format\n";
        return false;
    }    

    // Return true to indicate success
    return true;
}