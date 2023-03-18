//  LinuxGTKRenderer.cpp    first written by sebhall in March 2023

#include <LinuxGTKHost.h>
#include <LinuxGTKRenderer.h>
#include <UniversalUI/Angelo/CoreRenderer.h>


#include <AngeloCoreFragment.h>
#include <AngeloCoreVertex.h>

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Angelo/aPixelBuffer.h>


//  include platform-specific libraries
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <epoxy/gl.h>

//  include standard C++ library functions
#include <iostream>
#include <stdio.h>

bool CompilerShader(const char* vertex, const char* fragment, unsigned int &shaderProgram) {
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

//  compiler shaders and prepare for rendering
bool LinuxGTKRenderer::InitialiseRenderer() {
    
    if (!CompilerShader(AngeloCoreVertex, AngeloCoreFragment, genericShader)) {
        return false;
    }

    printf("SHADER COMPILATION SUCCEEDED\n");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         1.0f,  1.0f, // bottom left  
         1.0f, -1.0f, // bottom right 
        -1.0f,  1.0f, // top left   
         1.0f, -1.0f, // bottom right 
        -1.0f, -1.0f, // top right
        -1.0f,  1.0f  // top left   
    };

    unsigned int VBO;
    glGenVertexArrays(1, &genericVertexArray);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(genericVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    glDisable(GL_CULL_FACE);

    return true;
}

//  render a view, override by platform for OpenGL/Metal functionality
void LinuxGTKRenderer::RenderView(uView* view) {

    for (uView* subview : view->subviews) {
        RenderView(subview);
    }

    // redraw persistent views if anything changed.
    if (view->persistent && view->needsRedraw) {
        // ANGELO MAIN DRAW METHOD
        printf("ANGELO-INFO: rendered to texture\n");
        view->needsRedraw = false;
    }
    
    if (view->persistent) {
        // RENDER TEXTURE TO SCREEN
        printf("ANGELO-INFO: render texture to screen\n");
        return;
    }

    // RENDER COMMAND TO VIEW FRAME
  
    glUseProgram(genericShader);
    glBindVertexArray(genericVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    printf("ANGELO-INFO: rendered view to screen\n");
}

//  render a window, override by platform for OpenGL/Metal functionality
void LinuxGTKRenderer::RenderWindow(uWindow* window) {
    printf("BEGINNING WINDOW RENDER FOR %s.\n", window->title.c_str());
    InitialiseRenderer();
    SetupViewForRendering(window->rootView);
    RenderView(window->rootView);
}

//  generate a new pixel buffer in GPU memory with a given size
aPixelBuffer LinuxGTKRenderer::NewPixelBuffer(uSize size) {
    aPixelBuffer newBuffer;
    newBuffer.size = size;
    glGenTextures(1, (unsigned int*) &newBuffer.id);
    glBindTexture(GL_TEXTURE_2D, newBuffer.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) size.width, (int) size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return newBuffer;
}

//  resize an existing pixel buffer
void LinuxGTKRenderer::ResizePixelBuffer(aPixelBuffer buffer, uSize size) {
    glBindTexture(GL_TEXTURE_2D, buffer.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) size.width, (int) size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//  fill a buffer with a given colour - use mainly for testing purposes
void LinuxGTKRenderer::ClearBuffer(aPixelBuffer buffer, uColour colour) {
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffer.id, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    glViewport(0, 0, (int) buffer.size.width, (int) buffer.size.height);
    glClearColor(colour.r, colour.g, colour.b, colour.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    printf("cleared buffer\n");
}