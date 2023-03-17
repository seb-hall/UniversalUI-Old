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
    unsigned int VAO, VBO, pixelbuffer, framebuffer;
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
    ShowWindow(pack->systemWindow, SW_SHOW);
}

void WinHost::ShowWindow(uWindow* window) {

    //  instantiate a new windowPack and show on screen
    SystemWindowPack* pack = new SystemWindowPack;
    windowMap[window] = pack;
    pack->window = window;
    DeployWindowPack(pack);
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