#include <gtk/gtk.h>
#include <iostream>
#include <vector>


using namespace std;

static cairo_surface_t *surface = NULL;
GtkBuilder *gtkBuilder;
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkWidget *windowInsertion;

/*Clear the surface, removing the scribbles*/
static void clear_surface (){
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}

/*Creates the surface*/
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data){
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));
  clear_surface ();
  return TRUE;
}

/* Redraw the screen from the surface */
static gboolean draw_cb (GtkWidget *widget, cairo_t   *cr,  gpointer   data){
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

/*Function that will be called when the ok button is pressed*/
 extern "C" G_MODULE_EXPORT void btn_ok_clicked_cb(){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_move_to(cr, 200, 100);
  cairo_line_to(cr, 300, 50);
  cairo_stroke(cr);
  gtk_widget_queue_draw (window_widget);
 } 

 extern "C" G_MODULE_EXPORT void insert_new_window () {
  gtk_widget_show(windowInsertion);
}

extern "C" G_MODULE_EXPORT void btn_ok_insert_point(){
  GtkEntry *NewPointName =  GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NewPointName"));
  GtkEntry *XPoint =  GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "XPoint"));
  GtkEntry *YPoint =  GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "YPoint"));

  const char *PointName = gtk_entry_get_text (NewPointName);
  const char *XPointAux = gtk_entry_get_text (XPoint);
  const char *YPointAux = gtk_entry_get_text (YPoint);

  double XPointDouble = atof(XPointAux);
  double YPointDouble = atof(YPointAux);
  cout << XPointDouble;
  //printf("%f\n", YPointDouble);
  
  // Continuar ...
}


int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  gtkBuilder = gtk_builder_new();
  gtk_builder_add_from_file(gtkBuilder, "window.glade", NULL);

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
  windowInsertion = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowInsertion") );
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  gtk_builder_connect_signals(gtkBuilder, NULL);
  gtk_widget_show_all(window_widget);
  gtk_main ();
  return 0;
}
 

