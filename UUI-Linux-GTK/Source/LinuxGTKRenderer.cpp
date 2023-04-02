//  LinuxGTKRenderer.cpp   first written by sebhall in March 2023

#include <LinuxGTKRenderer.h>

#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <AngeloCoreFragment.h>
#include <AngeloCoreVertex.h>

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <stdio.h>

#include <iostream>

//  test shader compilation etc
bool LinuxGTKRenderer::TestEnvironment() {
    return true;
}

bool LinuxGTKRenderer::SetupForRendering() {
    glGenFramebuffers(1, &FBO);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &AngeloCoreVertex, NULL);
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
    glShaderSource(fragmentShader, 1, &AngeloCoreFragment, NULL);
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
    CommandShader = glCreateProgram();
    glAttachShader(CommandShader, vertexShader);
    glAttachShader(CommandShader, fragmentShader);
    glLinkProgram(CommandShader);
    // check for linking errors
    glGetProgramiv(CommandShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(CommandShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    printf("Shaders compiled successfully.\n");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(0);

    return true;
}

//  render specified command
aPixelBuffer* LinuxGTKRenderer::RenderCommand(aRenderCommand command) {

    uSize extents = {100.0f, 100.0f};

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    aPixelBuffer* output = new aPixelBuffer;
    output->size = command.size;
    output->frame = {0.0, 0.0, command.size.width, command.size.height};
    glGenTextures(1, &output->id);
    glBindTexture(GL_TEXTURE_2D, output->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)command.size.width, (int)command.size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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
    glViewport(0, 0, (int)command.size.width, (int)command.size.height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT);

    
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
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDisable(GL_CULL_FACE);

    
    // RENDER COMMAND TO VIEW FRAME

    int opsSize = command.codes.size(); // get the size of the vector
    uint32_t* opsArray = &command.codes[0]; // get a pointer to the first element

    int idsSize = command.indices.size(); // get the size of the vector
    uint32_t* idsArray = &command.indices[0]; // get a pointer to the first element

    int pmsSize = command.parameters.size(); // get the size of the vector
    float* pmsArray = &command.parameters[0]; // get a pointer to the first element

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

    glUseProgram(CommandShader);

    glUniform1i(glGetUniformLocation(CommandShader, "ops"), 0);
    glUniform1i(glGetUniformLocation(CommandShader, "ids"), 1);
    glUniform1i(glGetUniformLocation(CommandShader, "pms"), 2);

    glUniform1i(glGetUniformLocation(CommandShader, "numIndices"), opsSize);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opsTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, idsTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pmsTex);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDeleteTextures(1, &opsTex);
    glDeleteTextures(1, &idsTex);
    glDeleteTextures(1, &pmsTex);

    

    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    return output;
}

//  render specified text
aPixelBuffer* LinuxGTKRenderer::RenderText(std::string text, float size) {

    // Draw some text on the surface
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 100.0, 100.0);
    cairo_t* cr = cairo_create(surface);
    cairo_select_font_face(cr, "Roboto", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, size);

    cairo_text_extents_t te;
    cairo_text_extents(cr, text.c_str(), &te);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, te.width, te.height);
    cr = cairo_create(surface);
    cairo_select_font_face(cr, "Roboto", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, size);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_move_to(cr, te.x_bearing*-1, te.y_bearing*-1);
    cairo_show_text(cr, text.c_str());

    printf("TE %f %f\n", te.width, te.x_advance);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    aPixelBuffer* output = new aPixelBuffer;
    output->size = {te.width, te.height};
    output->frame = {0.0, 0.0, te.width, te.height};
    glGenTextures(1, &output->id);
    glBindTexture(GL_TEXTURE_2D, output->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)te.width, (int)te.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, cairo_image_surface_get_data(surface));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output->id, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("UUI-ERROR: OpenGL framebuffer error\n");
    } else {
        printf("UUI-INFO: Compositor Framebuffer created successfully!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Clean up the cairo resources
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    return output;
}

//  render specified image
aPixelBuffer* LinuxGTKRenderer::RenderImage(std::string path) {

    // Load the png image file into a cairo surface
    cairo_surface_t* image = cairo_image_surface_create_from_png(path.c_str());

    // Check the status of the surface
    cairo_status_t status = cairo_surface_status(image);
    if (status == CAIRO_STATUS_FILE_NOT_FOUND) {
        printf("ANGELO-ERROR: image file not found\n");
    } else if (status == CAIRO_STATUS_READ_ERROR) {
        printf("ANGELO-ERROR: image file read failed\n");
    }

    // Get the width and height of the image
    int width = cairo_image_surface_get_width(image);
    int height = cairo_image_surface_get_height(image);

    // Create another cairo surface with the same size as the image
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

    // Create a cairo context for drawing on the surface
    cairo_t* cr = cairo_create(surface);

    // Draw the image on the surface
    cairo_set_source_surface(cr, image, 0, 0);
    cairo_paint(cr);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    aPixelBuffer* output = new aPixelBuffer;
    output->size = {(float) width, (float) height};
    glGenTextures(1, &output->id);
    glBindTexture(GL_TEXTURE_2D, output->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, cairo_image_surface_get_data(surface));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output->id, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("UUI-ERROR: OpenGL framebuffer error\n");
    } else {
        printf("UUI-INFO: Compositor Framebuffer created successfully!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    cairo_destroy(cr);
    cairo_surface_destroy(image);
    cairo_surface_destroy(surface);
    return output;
}
