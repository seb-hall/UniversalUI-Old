//  UniversalUI.cpp for UUI-Windows, first written by Seb H in February 2023
//  this file is the Windows specific implementation of the UniversalUI root
//  interface. There are two functions - UniversalUI() and uuiMain() - which
//  are the initialisation and event loop functions respectively.


#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/CoreHost.h>
#include <UniversalUI/Core/uSimpleApplication.h>
#include <UniversalUI/Core/uDesktopApplication.h>
//#include <UniversalUI/Core/uWindowManager.h>
//#include <UniversalUI/Angelo/aRenderer.h>

//  include standard C++ libraries

#include <stdio.h>
#include <map>
#include <string>

//  include GTK and EPOXY libraries for building GTK+ 3.0 OpenGL
//  enabled applications.

#include <gtk/gtk.h>
#include <epoxy/gl.h>

//  Initialise UniversalUI - checking for installation and user
//  aplication compatibility. If all is well, the function will
//  return true, and uuiMain should be called to start your app.
//  If the function returns false, there are errors in the host
//  platform i.e no OpenGL support or missing resources. The
//  int main() function should return at this point as UniversalUI
//  will not run correctly.


CoreHost host;

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


bool UniversalUI(uApplication* userApp) { 

    if (dynamic_cast<uSimpleApplication*>(userApp)) {
        printf("UUI-INFO: Simple Application Created\n");
    } else if (dynamic_cast<uDesktopApplication*>(userApp)) {
        printf("UUI-INFO: Desktop Application Created\n");
        uDesktopApplication* app = static_cast<uDesktopApplication*>(userApp);
        /*if (!app->windowManager) {
            printf("UUI-INFO: Generating default window manager...\n");
        }*/
        //app->windowManager = new uWindowManager;
       // app->windowManager->host = &host;
    } else {
        printf("UUI-CRITICAL: Invalid Application Created! Please use subclass either uSimpleApplication or uDesktopApplication.\n");
        return false;
    }

    printf("GOT TO HERE!\n");

    gtk_init(nullptr, nullptr);

    chWindowPack* pack = new chWindowPack;

    // create placeholder window for GL context
    pack->contextProvider = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_realize(pack->contextProvider);
    pack->glContext = gdk_window_create_gl_context(gtk_widget_get_window(pack->contextProvider), &pack->glError);
    //gdk_gl_context_set_required_version(pack->glContext, 1, 0);

    if (pack->glContext == nullptr) {
        printf("COREHOST: OpenGL context creation error!\n");
        return false;
    }

    /*if (gdk_gl_context_get_use_es(pack->glContext)) {
        printf("INFO: using OpenGL ES\n");
    } else {
        printf("INFO: using OpenGL desktop\n");
    }*/

    int major, minor;
    gdk_gl_context_get_version(pack->glContext, &major, &minor);
    printf("INFO: OpenGL initialised with version %u.%u\n", major, minor);

    gtk_main_quit();


    printf("\n\t*** Welcome to UniversalUI D3! ***\n\n");
    
    //host.app = userApp;
    //host.renderer = new aRenderer;
    return true;
}

//  Run UniversalUI. This function will only return after all
//  the other parts of the framework are terminated. You shouldn't
//  assume the function is returned and so the standard
//  implementation is to add 'return uuiMain(argc, argv);' as the
//  last line of your int main() function in main.cpp.



int uuiMain(int argc, char* argv[]) {

    gtk_init(&argc, &argv);

    /* APPLICATION LAUNCHED */

    gtk_main();

    printf("SUCCESS!");
    return 0;
}

