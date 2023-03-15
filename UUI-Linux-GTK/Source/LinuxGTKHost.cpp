#include <LinuxGTKHost.h>
#include <UniversalUI/Core/uWindow.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <epoxy/gl.h>

#include <stdio.h>

extern LinuxGTKHost* host;

struct chWindowPack {
    uWindow* window;
    GtkWidget* systemWindow;
    GtkWidget* canvas;
    GtkWidget* contextProvider;
    GdkGLContext* glContext;
    GError* glError;
    cairo_surface_t* cairoSurface;
    unsigned int VAO, VBO, pixelbuffer, framebuffer;

};

bool Draw(GtkWidget* widget, cairo_t* cairoContext, chWindowPack* pack);
void Destroy(chWindowPack* pack);
bool Configure(GtkWidget* widget, GdkEventConfigure *event, chWindowPack* pack);

void LinuxGTKHost::ShowWindow(uWindow* window) {

   // initialise new window pack
    chWindowPack* pack = new chWindowPack;

    // create placeholder window for GL context
    pack->contextProvider = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_realize(pack->contextProvider);
    pack->glContext = gdk_window_create_gl_context(gtk_widget_get_window(pack->contextProvider), &pack->glError);
    //gdk_gl_context_set_required_version(pack->glContext, 3, 3);

    if (pack->glContext == nullptr) {
        printf("COREHOST: OpenGL context creation error!\n");
        return;
    }

    //gdk_gl_context_realize(pack->glContext, &pack->glError);
    gdk_gl_context_make_current(pack->glContext);

    int major, minor;
    gdk_gl_context_get_version(pack->glContext, &major, &minor);
    printf("INFO: OpenGL initialised with version %d.%d\n", major, minor);

    
    /*if (gdk_gl_context_get_use_es(pack->glContext)) {
        printf("INFO: using OpenGL ES\n");
    } else {
        printf("INFO: using OpenGL desktop\n");
    }*/
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) window->size.width, (int) window->size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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

    glViewport(0, 0, (int) window->size.width, (int) window->size.height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    pack->window = window;
    pack->systemWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    pack->canvas = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(pack->systemWindow), pack->canvas);

    g_signal_connect(pack->canvas, "draw", G_CALLBACK(Draw), pack);
    g_signal_connect(pack->systemWindow, "destroy", G_CALLBACK(Destroy), pack);
    g_signal_connect(pack->canvas,"configure-event", G_CALLBACK(Configure), pack);

    gtk_window_set_title(GTK_WINDOW(pack->systemWindow), window->title.c_str());

    gtk_widget_show_all(pack->systemWindow);
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
    ShowWindow(window1);
    ShowWindow(window2);
    gtk_main();
    return 0;
}

bool Draw(GtkWidget* widget, cairo_t* cairoContext, chWindowPack* pack) { 
    //printf("INFO: draw window\n");

    gdk_gl_context_make_current(pack->glContext);
    glBindFramebuffer(GL_FRAMEBUFFER, pack->framebuffer);
    glBindTexture(GL_TEXTURE_2D, pack->pixelbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) pack->window->size.width, (int) pack->window->size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glViewport(0, 0, (int) pack->window->size.width, (int) pack->window->size.height);
    glClearColor(pack->window->background.r, pack->window->background.g, pack->window->background.b, pack->window->background.a);
    glClear(GL_COLOR_BUFFER_BIT);
    
    cairo_set_source_surface(cairoContext, pack->cairoSurface, 0, 0);
    gdk_cairo_draw_from_gl(cairoContext, gtk_widget_get_window(pack->contextProvider), pack->pixelbuffer, GL_TEXTURE, 1.0, 0, 0, (int) pack->window->size.width, (int) pack->window->size.height);
}

void Destroy(chWindowPack* pack) {
    printf("INFO: destroyed window\n");
    
}

bool Configure(GtkWidget* widget, GdkEventConfigure *event, chWindowPack* pack) { 
    gdk_gl_context_make_current(pack->glContext);
    //printf("INFO: configured window\n");
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    /// reset cairo surface
    if (pack->cairoSurface) {
        cairo_surface_destroy(pack->cairoSurface);
    }

    pack->cairoSurface = gdk_window_create_similar_surface(gtk_widget_get_window(widget), CAIRO_CONTENT_COLOR, width, height);

    pack->window->size = {(float) width, (float) height};
}