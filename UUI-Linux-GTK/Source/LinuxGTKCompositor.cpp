//  LinuxGTKCompositor.cpp   first written by sebhall in March 2023

#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Core/CoreGeometry.h>
#include <LinuxGTKCompositor.h>
#include <epoxy/gl.h>
#include <stdio.h>
#include <iostream>

bool LinuxGTKCompositor::SetupForCompositing() {

    glGenFramebuffers(1, &FBO);
    // Create a vertex array object and a vertex buffer object for the quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
    ShaderProgram = glCreateProgram();
    
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
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);
    glLinkProgram(ShaderProgram);

    // Check for linking errors
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the vertex and fragment shaders as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    return true;
}

//  render input buffers to output buffer
aPixelBuffer* LinuxGTKCompositor::CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers) {

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    aPixelBuffer* output = new aPixelBuffer;
    output->size = extents;
    glGenTextures(1, &output->id);
    glBindTexture(GL_TEXTURE_2D, output->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) extents.width, (int) extents.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, output->id, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("UUI-ERROR: OpenGL framebuffer error\n");
    } else {
        printf("UUI-INFO: Compositor Framebuffer created successfully!\n");
    }

    glViewport(0, 0, (int) extents.width, (int) extents.height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT);

    
    // Use the shader program
    glUseProgram(ShaderProgram);

    // Loop through the input buffers and render them to the result texture
    for (aPixelBuffer* buffer : inputBuffers) {
        // Bind the input texture
        glBindTexture(GL_TEXTURE_2D, buffer->id);

        // Set the viewport to match the input frame
        glViewport((int)buffer->frame.x, (int)buffer->frame.y, (int)buffer->frame.width, (int)buffer->frame.height);

        // Draw the quad with the input texture
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return output;
}