//  LinuxGTKHost.cpp    first written by sebhall in March 2023

//  include UniversalUI header files
#include <LinuxGTKHost.h>
#include <UniversalUI/Core/uApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uWindow.h>

#include <UniversalUI/Angelo/CoreAngelo.h>
#include <UniversalUI/Angelo/CoreRenderer.h>
#include <UniversalUI/Angelo/CoreCompositor.h>

//  include standard C++ libraries
#include <stdio.h>
#include <string>
#include <map>

//  include platform-specific libraries
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <epoxy/gl.h>

//  reference to global host pointer
extern LinuxGTKHost* host;

//  structure for GTK/Cairo/OpenGL/UniversalUI windows
struct SystemWindowPack {
    uWindow* window;
    GtkWidget* gtkWindow;
    GtkWidget* contextProvider;
    GtkWidget* canvas;
    GdkGLContext* glContext;
    cairo_surface_t* cairoSurface = nullptr;
    unsigned int VAO, VBO, pixelbuffer, framebuffer;
};

//  map of uWindows to SystemWindowPack for getting GTK objects for a given uWindow
std::map<uWindow*, SystemWindowPack*> windowMap = { };

//  forward definition of callbacks
bool DrawCallback(GtkWidget* widget, cairo_t* cairoContext, SystemWindowPack* pack);
void DestroyCallback(SystemWindowPack* pack);
bool ConfigureCallback(GtkWidget* widget, GdkEventConfigure *event, SystemWindowPack* pack);

//  takes a pointer to a windowPack and creates new GTK & OpenGL resources
void DeployWindowPack(SystemWindowPack* pack) {

    // Create a new GTK window
    pack->gtkWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pack->gtkWindow), pack->window->title.c_str());
    gtk_window_set_default_size(GTK_WINDOW(pack->gtkWindow), (int) pack->window->size.width, (int) pack->window->size.height);

    // Create a new GTK drawing area
    pack->canvas = gtk_drawing_area_new();

    gtk_widget_set_hexpand(pack->canvas, TRUE);
    gtk_widget_set_vexpand(pack->canvas, TRUE);

    // Add the drawing area to the window
    gtk_container_add(GTK_CONTAINER(pack->gtkWindow), pack->canvas);

    //  generate a new context provider so that the real GTKwindow is happy and
    //  behaves as expected
    pack->contextProvider = gtk_window_new(GTK_WINDOW_POPUP);

    // Realize the context provider to create a GdkWindow and an OpenGL context
    gtk_widget_realize(pack->contextProvider);
    
    // Get the GdkWindow of the drawing area
    GdkWindow *gdk_window = gtk_widget_get_window(pack->contextProvider);

    // Create an OpenGL context with version 3.3 and core profile
    pack->glContext = gdk_window_create_gl_context(gdk_window, NULL);
    gdk_gl_context_set_required_version(pack->glContext, 3, 3);
    
    // Make the OpenGL context current
    gdk_gl_context_make_current(pack->glContext);

    int major, minor;
    gdk_gl_context_get_version(pack->glContext, &major, &minor);
    printf("UUI-INFO: OpenGL initialised with version %d.%d\n", major, minor);

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
        printf("UUI-ERROR: OpenGL framebuffer error\n");
        return;
    }

    glViewport(0, 0, (int) pack->window->size.width, (int) pack->window->size.height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    host->angelo->compositor->SetupForCompositing();

    // Set up the draw signal handler for the drawing area
    g_signal_connect(pack->canvas, "draw", G_CALLBACK(DrawCallback), pack);
    g_signal_connect(pack->gtkWindow, "destroy", G_CALLBACK(DestroyCallback), pack);
    g_signal_connect(pack->canvas,"configure-event", G_CALLBACK(ConfigureCallback), pack);

    // Show the window and all its widgets
    gtk_widget_show_all(pack->gtkWindow);  

}

void LinuxGTKHost::ShowWindow(uWindow* window) {

    //  instantiate a new windowPack and show on screen
    SystemWindowPack* pack = new SystemWindowPack;
    windowMap[window] = pack;
    pack->window = window;
    DeployWindowPack(pack);
    //host->renderer->SetupWindowForRendering(window);
    //host->renderer->InitialiseRendererForWindow(window);
}

bool LinuxGTKHost::TestEnvironment() {

    // initialise GTK with no arguments
    gtk_init(nullptr, nullptr);

    //  create test window and check for valid context creation
    GtkWidget* testWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_realize(testWindow);
    
    GdkGLContext* testContext = gdk_window_create_gl_context(gtk_widget_get_window(testWindow), nullptr);
    gdk_gl_context_set_required_version(testContext, 3, 3);

    if (testContext == nullptr) {
        printf("UUI-ERROR: OpenGL init failed\n");
        return false;
    }

    //  assuming OpenGL is all set up ok, check version and destroy test resources
    gdk_gl_context_make_current(testContext);
    int major, minor;
    gdk_gl_context_get_version(testContext, &major, &minor);
    printf("UUI-INFO: OpenGL init with version %u.%u\n", major, minor);

    if (!host->angelo->renderer->TestEnvironment()) {
        printf("UUI-ERROR: shader compilation failed\n");
    }

    gtk_widget_destroy(testWindow);

    return true;
}

int LinuxGTKHost::main() {

    // run GTK main function. Will never return but feels wrong to have a void main function
    gtk_main();
    return 0;
}

void LinuxGTKHost::SetTitle(uWindow* window, std::string title) {
    //printf("SET TITLE\n");
    gtk_window_set_title(GTK_WINDOW(windowMap[window]->gtkWindow), title.c_str());
    window->title = title;
}

//  GTK draw callback
bool DrawCallback(GtkWidget* widget, cairo_t* cairoContext, SystemWindowPack* pack) { 

    //  get background colour of widget
    GdkRGBA color;
    GtkStyleContext* styleContext = gtk_widget_get_style_context(widget);

    //  get width and height of widget
    int width, height;
    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    //  apply background colour to widget
    gtk_render_background(styleContext, cairoContext, 0, 0, width, height);
    gtk_style_context_get_color(styleContext, gtk_style_context_get_state (styleContext), &color);
    gdk_cairo_set_source_rgba(cairoContext, &color);
    cairo_fill(cairoContext);


    //  set glContext to current
    gdk_gl_context_make_current(pack->glContext);

    //  bind framebuffer and resize GL Image
    /*glBlendFunc(GL_BLEND, GL_ONE_MINUS_SRC_ALPHA);
    glBindFramebuffer(GL_FRAMEBUFFER, pack->framebuffer);
    glBindTexture(GL_TEXTURE_2D, pack->pixelbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) pack->window->size.width, (int) pack->window->size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pack->pixelbuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("UUI-ERROR: OpenGL framebuffer error\n");
        return false;
    }

    // fill buffer with black
    glViewport(0, 0, (int) pack->window->size.width, (int) pack->window->size.height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClearColor(pack->window->background.r, pack->window->background.g, pack->window->background.b, pack->window->background.a);
    glClear(GL_COLOR_BUFFER_BIT);*/

    //  render window
    //host->renderer->RenderWindow(pack->window);

    //  unbind framebuffer and image
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    aPixelBuffer* pb = host->angelo->compositor->CompositeBuffers({(float)width, (float)height}, { });
    //  draw GL Image to cairo surface
    gdk_cairo_draw_from_gl(cairoContext, gtk_widget_get_window (pack->contextProvider), pb->id, GL_TEXTURE, 1.0, 0, 0, width, height);

    host->angelo->DestroyPixelBuffer(pb);

    return false;
} 

void DestroyCallback(SystemWindowPack* pack) {
    printf("UUI-INFO: destroyed window\n");
}

bool ConfigureCallback(GtkWidget* widget, GdkEventConfigure *event, SystemWindowPack* pack) { 

    if (pack->cairoSurface)
        cairo_surface_destroy (pack->cairoSurface);

    pack->cairoSurface = gdk_window_create_similar_surface(gtk_widget_get_window(widget), CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));

    cairo_t *cr;

    cr = cairo_create(pack->cairoSurface);

    cairo_set_source_rgb(cr, 0.125, 0.125, 0.125);
    cairo_paint(cr);

    cairo_destroy(cr);

    int width = gtk_widget_get_allocated_width(pack->gtkWindow);
    int height = gtk_widget_get_allocated_height(pack->gtkWindow);

    uSize newSize = {(float) width, (float) height};

    if (pack->window->size.width != newSize.width || pack->window->size.height != newSize.height) {
        pack->window->size = newSize;
        pack->window->rootView->frame = {(float) 0, (float) 0, (float) width, (float) height};
        if (host->appType == desktop) {
            uDesktopApplication* app = static_cast<uDesktopApplication*>(host->app);
            app->WindowResized(pack->window, newSize);
        } else if (host->appType == simple) {
            uSimpleApplication* app = static_cast<uSimpleApplication*>(host->app);
            app->Resized(newSize);
        }
    }
    
    return true;
}
