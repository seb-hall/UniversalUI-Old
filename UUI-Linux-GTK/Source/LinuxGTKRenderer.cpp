//  LinuxGTKRenderer.cpp   first written by sebhall in March 2023

#include <LinuxGTKRenderer.h>

#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Core/CoreGeometry.h>

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <stdio.h>

//  test shader compilation etc
bool LinuxGTKRenderer::TestEnvironment() {
    return true;
}

bool LinuxGTKRenderer::SetupForRendering() {
    glGenFramebuffers(1, &FBO);
    return true;
}

//  render specified command
aPixelBuffer* LinuxGTKRenderer::RenderCommand(aRenderCommand command) {
    return new aPixelBuffer;
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
    
    /*

    cairo_font_extents_t fe;
    
    const char* alphabet = text.c_str();
    char letter[2];

    cairo_font_extents (cr, &fe);
    for (int i=0; i < strlen(alphabet); i++) {
        *letter = '\0';
        strncat (letter, alphabet + i, 1);

        cairo_text_extents (cr, letter, &te);
        cairo_move_to (cr, i + 0.5 - te.x_bearing - te.width / 2,
                0.5 - fe.descent + fe.height / 2);
        cairo_show_text (cr, letter);
    }*/

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

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, output->id, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

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
