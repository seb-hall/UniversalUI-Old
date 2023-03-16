#include <LinuxGTKHost.h>
#include <UniversalUI/Core/uWindow.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <epoxy/gl.h>

#include <stdio.h>

extern LinuxGTKHost* host;

struct SystemWindowPack {
    uWindow* window;
    GtkWidget* gtkWindow;
    GtkWidget* canvas;
    GdkGLContext* glContext;
    cairo_surface_t* cairoSurface;
    unsigned int VAO, VBO, pixelbuffer, framebuffer;
};

bool DrawCallback(GtkWidget* widget, cairo_t* cairoContext, SystemWindowPack* pack);
void DestroyCallback(SystemWindowPack* pack);
bool ConfigureCallback(GtkWidget* widget, GdkEventConfigure *event, SystemWindowPack* pack);

void NewGTKWindow(GtkWidget** window, GtkWidget** drawing_area, GdkGLContext** gl_context, SystemWindowPack* pack) {

    pack->cairoSurface = NULL;

    // Create a new GTK window
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(*window), pack->window->title.c_str());
    gtk_window_set_default_size(GTK_WINDOW(*window), 800, 600);

    // Create a new GTK drawing area
    *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(*drawing_area, 800, 600);

    //gtk_widget_set_hexpand(*drawing_area, TRUE);
    //gtk_widget_set_vexpand(*drawing_area, TRUE);

    // Add the drawing area to the window
    gtk_container_add(GTK_CONTAINER(*window), *drawing_area);

    // Realize the drawing area to create a GdkWindow and an OpenGL context
    gtk_widget_realize(*drawing_area);

    // Get the GdkWindow of the drawing area
    GdkWindow *gdk_window = gtk_widget_get_window(*drawing_area);

    // Create an OpenGL context with version 3.3 and core profile
    *gl_context = gdk_window_create_gl_context(gdk_window, NULL);
    gdk_gl_context_set_required_version(*gl_context, 3, 3);

    // Make the OpenGL context current
    gdk_gl_context_make_current(*gl_context);

    int major, minor;
    gdk_gl_context_get_version(*gl_context, &major, &minor);
    printf("INFO: OpenGL initialised with version %d.%d\n", major, minor);

    // Enable alpha blending

     glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &pack->VAO);
    glGenBuffers(1, &pack->VBO);
    glBindVertexArray(pack->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, pack->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create OpenGL buffers
    glGenFramebuffers(1, &pack->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, pack->framebuffer);
    glGenTextures(1, &pack->pixelbuffer);
    glBindTexture(GL_TEXTURE_2D, pack->pixelbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) pack->window->size.width, (int) pack->window->size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pack->pixelbuffer, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("FRAMEBUFFER NOT GOOD\n");
    } else {
        printf("FRAMEBUFFER OK!\n");
    }

    // fill buffer with black
    glViewport(0, 0, (int) pack->window->size.width, (int) pack->window->size.height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Set up the draw signal handler for the drawing area
    g_signal_connect(pack->canvas, "draw", G_CALLBACK(DrawCallback), pack);
    g_signal_connect(pack->gtkWindow, "destroy", G_CALLBACK(DestroyCallback), pack);
    g_signal_connect(pack->canvas,"configure-event", G_CALLBACK(ConfigureCallback), pack);

    // Show the window and all its widgets
    gtk_widget_show_all(*window);  

}

void LinuxGTKHost::ShowWindow(uWindow* window) {
    SystemWindowPack* pack = new SystemWindowPack;
    pack->window = window;
    NewGTKWindow(&pack->gtkWindow, &pack->canvas, &pack->glContext, pack);

}

bool LinuxGTKHost::TestEnvironment() {
    gtk_init(nullptr, nullptr);

    GtkWidget* testWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_realize(testWindow);
    
    GdkGLContext* testContext = gdk_window_create_gl_context(gtk_widget_get_window(testWindow), nullptr);
    gdk_gl_context_set_required_version(testContext, 3, 3);

    if (testContext == nullptr) {
        printf("ANGELO: OpenGL init failed\n");
        return false;
    }

    gdk_gl_context_make_current(testContext);
    int major, minor;
    gdk_gl_context_get_version(testContext, &major, &minor);
    printf("UUI-INFO: OpenGL init with version %u.%u\n", major, minor);

    gtk_widget_destroy(testWindow);

    return true;
}

int LinuxGTKHost::main() {
    uWindow* window1 = new uWindow;
    window1->background = {1.0, 0.0, 0.0, 1.0};
    uWindow* window2 = new uWindow;
    window2->background = {0.0, 1.0, 0.0, 1.0};
    uWindow* window3 = new uWindow;
    window3->background = {0.0, 0.0, 1.0, 1.0};

    ShowWindow(window1);
    //ShowWindow(window2);
    //ShowWindow(window3);

    gtk_main();
    return 0;
}

bool DrawCallback(GtkWidget* widget, cairo_t* cairoContext, SystemWindowPack* pack) { 
    printf("INFO: draw window\n");

    cairo_set_source_surface (cairoContext, pack->cairoSurface, 0, 0);
    
    if (pack->glContext == NULL) {
        printf("OOPS!\n");
    }

    int width = gtk_widget_get_allocated_width(pack->gtkWindow);
    int height = gtk_widget_get_allocated_height(pack->gtkWindow);

    /*printf("%d, %d\n", width, height);

    pack->window->size = {(float) width, (float) height};

    printf("%f, %f\n", pack->window->size.width, pack->window->size.height);

    gdk_gl_context_make_current(pack->glContext);
    glBindFramebuffer(GL_FRAMEBUFFER, pack->framebuffer);
    glBindTexture(GL_TEXTURE_2D, pack->pixelbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glViewport(0, 0, width, height);
    glClearColor(pack->window->background.r, pack->window->background.g, pack->window->background.b, pack->window->background.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // RENDER HERE

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    //cairo_set_source_surface(cairoContext, pack->cairoSurface, 0, 0);
    gdk_cairo_draw_from_gl(cairoContext, gtk_widget_get_window (pack->gtkWindow), pack->pixelbuffer, GL_TEXTURE, 1.0, 0, 0, width, height);
    //cairo_surface_flush(pack->cairoSurface);*/

    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    // The texture we're going to render to
    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("FRAMEBUFFER NOT GOOD\n");
    } else {
    printf("FRAMEBUFFER OK!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0,0,width,height); // Render on the whole framebuffer, complete from the lower left corner to the upper right


    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height); 

    gdk_cairo_draw_from_gl(cairoContext, gtk_widget_get_window (pack->gtkWindow), renderedTexture, GL_TEXTURE, 1.0, 0, 0, width, height);

    GdkGLDrawable *gl_drawable = gdk_gl_drawable_get_current ();

    // Swap buffers
    gdk_gl_drawable_swap_buffers (gl_drawable);

    return false;
} 

/*bool DrawCallback(GtkWidget* widget, cairo_t* cairoContext, SystemWindowPack* pack) { 
    printf("INFO: draw window\n");

    cairo_set_source_surface (cairoContext, pack->cairoSurface, 0, 0);
    //cairo_paint (cr);
    printf("draw!\n");
    
    if (pack->glContext == NULL) {
        printf("OOPS!\n");
    }

    cairo_set_source_rgb (cairoContext, 1, 1, 1);
    cairo_paint (cairoContext);

    return false;
}*/

void DestroyCallback(SystemWindowPack* pack) {
    printf("INFO: destroyed window\n");
    
}

bool ConfigureCallback(GtkWidget* widget, GdkEventConfigure *event, SystemWindowPack* pack) { 

     if (pack->cairoSurface)
        cairo_surface_destroy (pack->cairoSurface);

    pack->cairoSurface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));

    cairo_t *cr;

    cr = cairo_create(pack->cairoSurface);

    cairo_set_source_rgb(cr, 0.125, 0.125, 0.125);
    cairo_paint(cr);

    cairo_destroy(cr);

    printf("INFO: configured window\n");

    
    int width = gtk_widget_get_allocated_width(pack->gtkWindow);
    int height = gtk_widget_get_allocated_height(pack->gtkWindow);

    pack->window->size = {(float) width, (float) height};
    //cairo_surface_mark_dirty(pack->cairoSurface);

    printf("all done!\n");

    return true;
}