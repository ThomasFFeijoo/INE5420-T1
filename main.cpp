#include <gtk/gtk.h>
#include <iostream>
#include <vector>

#include "Ponto.hpp"
#include "Linha.hpp"
#include "Poligono.hpp"
#include "DisplayFile.hpp"
#include "Viewport.hpp"
#include "Window.hpp"


using namespace std;

static cairo_surface_t *surface = NULL;
GtkBuilder *gtkBuilder;
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkWidget *windowInsertion;

DisplayFile *displayFile;
Viewport *viewportP;
Window *windowP;

std::vector<Coordenadas> wireframeCoords;

GtkTreeStore *store;
GtkWidget *treeViewList;
GtkTreeViewColumn *column;
GtkCellRenderer *renderer;
enum{
  NOME_OBJETO,
  TIPO_OBJETO
};

GtkTextBuffer *buffer;
GtkTextView *outputCommandsShell;

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

static gboolean drawWindow (GtkWidget *widget, cairo_t *cr, gpointer data){
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
  return FALSE;
}

/* Redraw the screen from the surface */
void repaintWindow (){
  cairo_t *cr;
  clear_surface();
  cr = cairo_create (surface);
  viewportP->transformada(cr, *(windowP->getInicioDaWindow()), *(windowP->getFimDaWindow()), displayFile);
  gtk_widget_queue_draw (drawing_area);
}

void setupTree(){
  store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
  treeViewList = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
  g_object_unref (G_OBJECT (store));
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Nome", renderer, "text", NOME_OBJETO, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeViewList), column);
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Tipo", renderer, "text", TIPO_OBJETO, NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (treeViewList), column);
}

void printCommandLogs(const char* text) {
  GtkTextIter it;
  GtkTextMark *textMarks = gtk_text_buffer_get_insert(buffer);
  gtk_text_buffer_get_iter_at_mark (buffer, &it, textMarks);
  gtk_text_buffer_insert(buffer, &it, text, -1);
  gtk_text_view_scroll_to_mark(outputCommandsShell, textMarks, 0, false, 0, 0);
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
  printCommandLogs("btn_ok_insert_point\n");
  GtkEntry *NewPointName =  GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NewPointName"));
  GtkEntry *XPoint =  GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "XPoint"));
  GtkEntry *YPoint =  GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "YPoint"));

  const char *PointName = gtk_entry_get_text (NewPointName);
  const char *XPointAux = gtk_entry_get_text (XPoint);
  const char *YPointAux = gtk_entry_get_text (YPoint);

  double XPointDouble = atof(XPointAux);
  double YPointDouble = atof(YPointAux);
  
  Ponto * ponto = new Ponto(PointName, "Ponto", std::vector<Coordenadas>({Coordenadas(XPointDouble, YPointDouble, 0, 0)}));
  gtk_widget_hide(windowInsertion);
  displayFile->addObjectInTheWorld(ponto);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_ok_insert_line(){
  printCommandLogs("btn_ok_insert_line_actived\n");
  GtkEntry *entryNameNewLine = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryNameNewLine"));
  GtkEntry *entryX1Line = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryX1Line"));
  GtkEntry *entryY1Line = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryY1Line"));

  GtkEntry *entryX2Line = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryX2Line"));
  GtkEntry *entryY2Line = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryY2Line"));


  const char *entryLineName = gtk_entry_get_text (entryNameNewLine);
  const char *entryX1LineAux = gtk_entry_get_text (entryX1Line);
  const char *entryY1LineAux = gtk_entry_get_text (entryY1Line);

  const char *entryX2LineAux = gtk_entry_get_text (entryX2Line);
  const char *entryY2LineAux = gtk_entry_get_text (entryY2Line);


  double X1Line = atof(entryX1LineAux);
  double Y1Line = atof(entryY1LineAux);

  double X2Line= atof(entryX2LineAux);
  double Y2Line= atof(entryY2LineAux);

  Linha * linha = new Linha(entryLineName, "Linha", std::vector<Coordenadas>({Coordenadas(X1Line, Y1Line, 0, 0),Coordenadas(X2Line, Y2Line, 0, 0)}));
  gtk_widget_hide(windowInsertion);
  displayFile->addObjectInTheWorld(linha);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_ok_insert_wireframe(){
  printCommandLogs("btn_ok_insert_wireframe_actived\n");

  GtkEntry *entryNameNewWireframe = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryNameNewWireframe"));
  const char *entryWireframeName = gtk_entry_get_text (entryNameNewWireframe);
  wireframeCoords.push_back(wireframeCoords.front());
  Poligono * poligono = new Poligono(entryWireframeName, "Poligono", wireframeCoords);
  gtk_widget_hide(windowInsertion);
  displayFile->addObjectInTheWorld(poligono);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_ok_insert_coords_wireframe(){
  printCommandLogs("btn_ok_insert_coords_wireframe_actived\n");
  
  GtkEntry *entryX1Wireframe = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryX1Wireframe"));
  GtkEntry *entryY1Wireframe = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryY1Wireframe"));

  const char *entryX1WireframeAux = gtk_entry_get_text (entryX1Wireframe);
  const char *entryY1WireframeAux = gtk_entry_get_text (entryY1Wireframe);

  double X1Wireframe = atof(entryX1WireframeAux);
  double Y1Wireframe = atof(entryY1WireframeAux);

  wireframeCoords.push_back(Coordenadas(X1Wireframe, Y1Wireframe, 0.0, 0.0));
}

extern "C" G_MODULE_EXPORT void get_text_step(){
  printCommandLogs("get_text_step\n");
  GtkEntry *entryStep = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryStepSize"));
  const char *entryStepText = (char*) gtk_entry_get_text (entryStep);
  printf("Step: %d\n", atoi(entryStepText) );
}

extern "C" G_MODULE_EXPORT void get_text_degrees(){
  printCommandLogs("get_text_degrees\n");
  GtkEntry *entryDegrees = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryDegreesSize"));
  const char *entryDegreesText = gtk_entry_get_text (entryDegrees);
}

extern "C" G_MODULE_EXPORT void btn_zoom_in_clicked(){
  printCommandLogs("btn_zoom_in_clicked\n");
  windowP->zoom(1.1);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_zoom_out_clicked(){
  printCommandLogs("btn_zoom_out_clicked\n");
  windowP->zoom(0.9);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_up_clicked(){
  printCommandLogs("btn_up_clicked\n");
  windowP->mover(0,10,0);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_down_clicked(){
  printCommandLogs("btn_down_clicked\n");
  windowP->mover(0,-10,0);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_left_clicked(){
  printCommandLogs("btn_left_clicked\n");
  windowP->mover(-10,0,0);
  repaintWindow ();
}

extern "C" G_MODULE_EXPORT void btn_right_clicked(){
  printCommandLogs("btn_right_clicked\n");
  windowP->mover(10,0,0);
  repaintWindow ();
}

int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  gtkBuilder = gtk_builder_new();
  gtk_builder_add_from_file(gtkBuilder, "window.glade", NULL);

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
  windowInsertion = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowInsertion") );
  outputCommandsShell = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "OutputCommandsShell"));

  buffer = gtk_text_buffer_new(NULL);
  gtk_text_view_set_buffer(outputCommandsShell, buffer);
  gtk_text_view_set_wrap_mode(outputCommandsShell, GTK_WRAP_NONE);

  Coordenadas inicio = Coordenadas(0.0,0.0,0.0,0.0);
  Coordenadas fim = Coordenadas(300.0,300.0,0.0,0.0);
  DisplayFile dp = DisplayFile();
  displayFile = &dp;
  Viewport vp = Viewport();
  viewportP = &vp;
  Window cWindow = Window(&inicio, &fim, displayFile);
  windowP = &cWindow;

  setupTree();

  g_signal_connect (drawing_area, "draw", G_CALLBACK (drawWindow), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  gtk_builder_connect_signals(gtkBuilder, NULL);
  gtk_widget_show_all(window_widget);
  gtk_main ();
  return 0;
}
 

