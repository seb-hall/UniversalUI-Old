#include <gtk/gtk.h>
#include <epoxy/gl.h>

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
    GdkGLContext* context;
    GLuint latest;
    GtkWidget* window2;
    GError* contextError;

static void clear_surface(void) {
  cairo_t *cr;

  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 0.125, 0.125, 0.125);
  cairo_paint(cr);

  cairo_destroy(cr);
}

int width, height;

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb (GtkWidget* widget, GdkEventConfigure *event, gpointer data) {

   if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));

  /* Initialize the surface to white */
  clear_surface ();

  width = gtk_widget_get_allocated_width (widget);

  height = gtk_widget_get_allocated_height (widget);

  printf("new size %d, %d\n", width, height);

  //GdkGLContext* context = gdk_surface_create_gl_context()
  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb (GtkWidget* widget, cairo_t* cr, gpointer data) {
    
  cairo_set_source_surface (cr, surface, 0, 0);
  //cairo_paint (cr);
  printf("draw!\n");
  
  if (context == NULL) {
    printf("OOPS!\n");
  }
  
  //gdk_context_destr
  //gdk_gl_context_make_current(context);

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

  gdk_cairo_draw_from_gl(cr, gtk_widget_get_window (window2), renderedTexture, GL_TEXTURE, 1.0, 0, 0, width, height);

  return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget* widget, gdouble x, gdouble y) {
  cairo_t *cr;

  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

  cairo_rectangle (cr, x - 3, y - 3, 6, 6);
  cairo_fill (cr);

  cairo_destroy (cr);

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw_area (widget, x - 3, y - 3, 6, 6);
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer data) {
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->button == GDK_BUTTON_PRIMARY)
    {
      draw_brush (widget, event->x, event->y);
    }
  else if (event->button == GDK_BUTTON_SECONDARY)
    {
      clear_surface ();
      gtk_widget_queue_draw (widget);
    }

  /* We've handled the event, stop processing */
  return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget* widget, GdkEventMotion *event, gpointer data) {
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    draw_brush (widget, event->x, event->y);

  /* We've handled it, stop processing */
  return TRUE;
}

static void close_window (void) {
  if (surface)
    cairo_surface_destroy (surface);
}

static void activate (GtkApplication* app, gpointer user_data) {

    GtkWidget *window;
    GtkWidget *canvas;


    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "UUI Window (Test Worked)");

    g_signal_connect (window, "destroy", G_CALLBACK(close_window), NULL);

    //gtk_widget_realize(window);
    //
    
    canvas = gtk_drawing_area_new();
  
    gtk_container_add(GTK_CONTAINER (window), canvas);

    // set minimum size
    gtk_widget_set_size_request(canvas, 800, 600);

    g_signal_connect(canvas, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(canvas,"configure-event", G_CALLBACK(configure_event_cb), NULL);

    g_signal_connect(canvas, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
    g_signal_connect(canvas, "button-press-event", G_CALLBACK(button_press_event_cb), NULL);

    gtk_widget_set_events (canvas, gtk_widget_get_events(canvas) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

    gtk_widget_show_all (window);

    window2 = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_realize(window2);
    context = gdk_window_create_gl_context(gtk_widget_get_window(window2), &contextError);
    gdk_gl_context_make_current(context);
    //gdk_gl_context_make_current(NULL);

}

int main (int argc, char** argv) {
  GtkApplication* app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
