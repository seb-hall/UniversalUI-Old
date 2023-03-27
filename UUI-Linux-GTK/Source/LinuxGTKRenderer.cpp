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
#include <cstdint>
#include <map>

struct WindowRenderPack {
    unsigned int ShaderProgram;
    unsigned int VertexArray;

};

std::map<uWindow*, WindowRenderPack> renderPackMap;

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

    

    return true;
}

//  render a view, override by platform for OpenGL/Metal functionality
void LinuxGTKRenderer::RenderView(uView* view, uWindow* parentWindow) {

    for (uView* subview : view->subviews) {
        RenderView(subview, parentWindow);
    }

    // redraw persistent views if anything changed.
    if (view->persistent && view->needsRedraw) {
        // ANGELO MAIN DRAW METHOD
        printf("ANGELO-INFO: rendered to texture\n");
        view->needsRedraw = false;
    } else if (view->persistent) {
        // RENDER TEXTURE TO SCREEN
        printf("ANGELO-INFO: render texture to screen\n");
        return;
    }

    // RENDER COMMAND TO VIEW FRAME

    int opsSize = commands[view].codes.size(); // get the size of the vector
    uint32_t* opsArray = &commands[view].codes[0]; // get a pointer to the first element

    int idsSize = commands[view].indices.size(); // get the size of the vector
    uint32_t* idsArray = &commands[view].indices[0]; // get a pointer to the first element

    int pmsSize = commands[view].parameters.size(); // get the size of the vector
    float* pmsArray = &commands[view].parameters[0]; // get a pointer to the first element

    unsigned int opsTex;

    glGenTextures(1, &opsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opsTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, opsSize, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, opsSize, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, opsArray);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int idsTex;

    glGenTextures(1, &idsTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, idsTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, idsSize, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, idsSize, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, idsArray);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int pmsTex;

    glGenTextures(1, &pmsTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pmsTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, pmsSize, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pmsSize, 1, GL_RED, GL_FLOAT, pmsArray);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(renderPackMap[parentWindow].ShaderProgram);

    glUniform1i(glGetUniformLocation(renderPackMap[parentWindow].ShaderProgram, "ops"), 0);
    glUniform1i(glGetUniformLocation(renderPackMap[parentWindow].ShaderProgram, "ids"), 1);
    glUniform1i(glGetUniformLocation(renderPackMap[parentWindow].ShaderProgram, "pms"), 2);

    glUniform1i(glGetUniformLocation(renderPackMap[parentWindow].ShaderProgram, "numIndices"), opsSize);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opsTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, idsTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pmsTex);

    glBindVertexArray(renderPackMap[parentWindow].VertexArray);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteTextures(1, &opsTex);
    glDeleteTextures(1, &idsTex);
    glDeleteTextures(1, &pmsTex);

}

//  setup shaders and buffers for window
void LinuxGTKRenderer::InitialiseRendererForWindow(uWindow* window) {
    WindowRenderPack pack;
    renderPackMap[window] = pack;
    
    if (!CompilerShader(AngeloCoreVertex, AngeloCoreFragment, renderPackMap[window].ShaderProgram)) {
        return;
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
    glGenVertexArrays(1, &renderPackMap[window].VertexArray);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(renderPackMap[window].VertexArray);

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
}

//  render a window, override by platform for OpenGL/Metal functionality
void LinuxGTKRenderer::RenderWindow(uWindow* window) {
    //printf("BEGINNING WINDOW RENDER FOR %s.\n", window->title.c_str());
    SetupViewForRendering(window->rootView);
    RenderView(window->rootView, window);
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
    /*glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffer.id, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    glViewport(0, 0, (int) buffer.size.width, (int) buffer.size.height);
    glClearColor(colour.r, colour.g, colour.b, colour.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    */
    printf("cleared buffer\n");
}