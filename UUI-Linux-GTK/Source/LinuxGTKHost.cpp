#include <LinuxGTKHost.h>

#include <gtk/gtk.h>
#include <epoxy/gl.h>

#include <stdio.h>

extern LinuxGTKHost* host;

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
    gtk_main();
    return 0;
}