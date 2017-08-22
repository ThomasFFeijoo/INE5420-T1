#include <gtk/gtk.h>
#include <iostream>
#include <vector>

#include "Ponto.hpp"
#include "Linha.hpp"
#include "Poligono.hpp"
#include "DisplayFile.hpp"
#include "Viewport.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "Objeto.hpp"


using namespace std;

static cairo_surface_t *surface = NULL;

GtkBuilder *gtkBuilder;
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkWidget *windowInsertion;
GtkWidget *windowRemove;
GtkWidget *windowTranslacao;
GtkWidget *windowEscalona;
GtkWidget *windowListaObjetos;
GtkWidget *windowRotaciona;


DisplayFile *displayFile;
Viewport *viewportP;
Window *windowP;
World *world;


std::vector<Coordenadas> poligonoCoords;

GtkTextBuffer *buffer;
GtkTextView *outputCommandsShell;

Coordenadas inicio = Coordenadas(0.0,0.0,0.0,0.0);
Coordenadas fim = Coordenadas(400.0,400.0,0.0,0.0);


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

/**
 * Toda vez que a função draw é chamada, ativa essa função.
 */
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);

  DisplayFile * aux = world->getDisplayfile();
  std::unordered_map<std::string, Objeto*> objetos = aux->getAllObjectsFromTheWorld();

  std::string print = "";

  for (auto it = objetos.begin(); it != objetos.end(); ++it){
    print.append(it->second->getName());
    print.append("\n");
  }

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(windowListaObjetos));
  gtk_text_buffer_set_text(buffer, print.c_str(), -1);
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
  viewportP->transformada(cr, *(windowP->getInicioDaWindow()), *(windowP->getFimDaWindow()), world->getDisplayfile());
  gtk_widget_queue_draw (drawing_area);
}

void printCommandLogs(const char* text) {
  GtkTextIter it;
  GtkTextMark *textMarks = gtk_text_buffer_get_insert(buffer);
  gtk_text_buffer_get_iter_at_mark (buffer, &it, textMarks);
  gtk_text_buffer_insert(buffer, &it, text, -1);
  gtk_text_view_scroll_to_mark(outputCommandsShell, textMarks, 0, false, 0, 0);
}


/* Bota na tela a janela que receberá os parametros dos objetos*/
extern "C" G_MODULE_EXPORT void insert_new_object_window () {
  printCommandLogs("insert_new_object_window\n");
  gtk_widget_show(windowInsertion);
}

/* Fecha a tela que recebe os parametros*/
extern "C" G_MODULE_EXPORT void btn_cancel_insertion_actived () {
  printCommandLogs("btn_cancel_insertion_actived\n");
  gtk_widget_hide(windowInsertion);
}

extern "C" G_MODULE_EXPORT void remove_object_window () {
  printCommandLogs("remove_object_window\n");
  gtk_widget_show(windowRemove);
}

extern "C" G_MODULE_EXPORT void btn_cancel_translada_actived () {
  printCommandLogs("btn_cancel_translada_actived\n");
  gtk_widget_hide(windowTranslacao);
}

extern "C" G_MODULE_EXPORT void translada_object_window () {
  printCommandLogs("translada_object_window\n");
  gtk_widget_show(windowTranslacao);
}

extern "C" G_MODULE_EXPORT void btn_cancel_escala_actived () {
  printCommandLogs("btn_cancel_escala_actived\n");
  gtk_widget_hide(windowEscalona);
}

extern "C" G_MODULE_EXPORT void escala_object_window () {
  printCommandLogs("escala_object_window\n");
  gtk_widget_show(windowEscalona);
}

extern "C" G_MODULE_EXPORT void btn_cancel_rotaciona_actived () {
  printCommandLogs("btn_cancel_rotaciona_actived\n");
  gtk_widget_hide(windowRotaciona);
}

extern "C" G_MODULE_EXPORT void rotaciona_object_window () {
  printCommandLogs("rotaciona_object_window\n");
  gtk_widget_show(windowRotaciona);
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
  printf("Degrees: %f\n", atof(entryDegreesText));
}

extern "C" G_MODULE_EXPORT void btn_remove_object_actived(){
  printCommandLogs("btn_remove_object_actived\n");
  GtkEntry *entryStep = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "RemoveObjectName"));
  const char *entryStepText = (char*) gtk_entry_get_text (entryStep);
  gtk_widget_hide(windowRemove);
  if (!world->getDisplayfile()->isEmpty())
  {
    world->removeObjetosNoMundo(entryStepText);
    repaintWindow ();
  }
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

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "MainWindow") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
  windowInsertion = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowInsertion") );
  windowRemove = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "removeObject") );
  outputCommandsShell = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "OutputCommandsShell"));
  windowTranslacao = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowTranslacao"));
  windowEscalona = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowEscalonamento"));
  windowRotaciona = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "WindowRotaciona"));
  windowListaObjetos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "ObjectsInTheWorldInterface"));

  gtk_text_view_set_editable(GTK_TEXT_VIEW(windowListaObjetos), FALSE);
  buffer = gtk_text_buffer_new(NULL);
  gtk_text_view_set_buffer(outputCommandsShell, buffer);
  gtk_text_view_set_wrap_mode(outputCommandsShell, GTK_WRAP_NONE);

  DisplayFile dp = DisplayFile();
  displayFile = &dp;
  Viewport vp = Viewport(inicio, fim);
  viewportP = &vp;
  Window cWindow = Window(&inicio, &fim, displayFile);
  windowP = &cWindow;
  World wd = World();
  world = &wd;
  Transformacao2D t = Transformacao2D();
  transformador = &t;


  g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);
  g_signal_connect (drawing_area, "draw", G_CALLBACK (drawWindow), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);

  gtk_builder_connect_signals(gtkBuilder, NULL);
  gtk_widget_show_all(window_widget);
  gtk_main ();
  return 0;
}
 

