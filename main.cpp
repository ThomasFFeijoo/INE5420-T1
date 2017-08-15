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

std::vector<Coordenadas> poligonoCoords;

GtkTreeStore *store;
GtkWidget *treeViewList;
GtkTreeViewColumn *column;
GtkCellRenderer *renderer;
enum{
  NOME_OBJETO,
  TIPO_OBJETO
};


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


/* Faz a conversão da window */
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

/* Bota na tela a janela que receberá os parametros dos objetos*/
 extern "C" G_MODULE_EXPORT void insert_new_window () {
  gtk_widget_show(windowInsertion);
}

/* Fecha a tela que recebe os parametros*/
extern "C" G_MODULE_EXPORT void btn_cancel_insertion () {
  gtk_widget_hide(windowInsertion);
}

/* Adiciona um novo Ponto. Primeiro pega os objetos que contem as informações de nome e coordenadas. Após isso, extrai o texto dos objetos pegos. 
Converte as coords para double, cria um objeto Ponto passando os parametros pegos, adiciona ele ao displayfile.*/

extern "C" G_MODULE_EXPORT void btn_ok_insert_point(){
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

/* Adiciona uma nova Linha. Primeiro pega os objetos que contem as informações de nome e coordenadas. Após isso, extrai o texto dos objetos pegos. 
Converte as coords para double, cria um objeto Linha passando os parametros pegos, adiciona ele ao displayfile.*/

extern "C" G_MODULE_EXPORT void btn_ok_insert_line(){
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

/* Adiciona um novo poligono. Ele pega as coordenadas armazenadas pelo metodo 'btn_ok_insert_coords_poligono', cria um novo objeto Poligono e adiciona ao displayFile. */

extern "C" G_MODULE_EXPORT void btn_ok_insert_poligono(){

  GtkEntry *entryNameNewPoligono = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryNameNewPoligono"));
  const char *entryPoligonoName = gtk_entry_get_text (entryNameNewPoligono);
  poligonoCoords.push_back(poligonoCoords.front());
  Poligono * poligono = new Poligono(entryPoligonoName, "Poligono", poligonoCoords);
  gtk_widget_hide(windowInsertion);
  displayFile->addObjectInTheWorld(poligono);
  repaintWindow ();
}

/* Metodo para pegar as coordenadas que formarão um Poligono*/
extern "C" G_MODULE_EXPORT void btn_ok_insert_coords_poligono(){
  
  GtkEntry *entryX1Poligono = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryX1Poligono"));
  GtkEntry *entryY1Poligono = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EntryY1Poligono"));

  const char *entryX1PoligonoAux = gtk_entry_get_text (entryX1Poligono);
  const char *entryY1PoligonoAux = gtk_entry_get_text (entryY1Poligono);

  double X1Poligono = atof(entryX1PoligonoAux);
  double Y1Poligono = atof(entryY1PoligonoAux);

  poligonoCoords.push_back(Coordenadas(X1Poligono, Y1Poligono, 0.0, 0.0));
}

/* Método que realiza zoom in na window */
extern "C" G_MODULE_EXPORT void btn_zoom_in_clicked(){
  windowP->zoom(1.1);
  repaintWindow ();
}

/* Método que realiza zoom out na window */
extern "C" G_MODULE_EXPORT void btn_zoom_out_clicked(){
  windowP->zoom(0.9);
  repaintWindow ();
}

/* Método que desloca os objetos para cima */
extern "C" G_MODULE_EXPORT void btn_up_clicked(){
  windowP->mover(0,-10,0);
  repaintWindow ();
}

/* Método que desloca os objetos para baixo */
extern "C" G_MODULE_EXPORT void btn_down_clicked(){
  windowP->mover(0,10,0);
  repaintWindow ();
}

/* Método que desloca os objetos para a esquerda */
extern "C" G_MODULE_EXPORT void btn_left_clicked(){
  windowP->mover(10,0,0);
  repaintWindow ();
}

/* Método que desloca os objetos para a direita */
extern "C" G_MODULE_EXPORT void btn_right_clicked(){
  windowP->mover(-10,0,0);
  repaintWindow ();
}

int main(int argc, char *argv[]){
  gtk_init(&argc, &argv);

  gtkBuilder = gtk_builder_new();
  gtk_builder_add_from_file(gtkBuilder, "window.glade", NULL);

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
  windowInsertion = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowInsertion") );

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
 

