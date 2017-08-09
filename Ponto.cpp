#include "Ponto.hpp"


void Ponto::desenhar(cairo_t* cr, std::vector<Coordenadas> coords){
	cairo_move_to(cr, coords[0].getX(), coords[0].getY());
	cairo_line_to (cr, coords[0].getX() + 1.0, coords[0].getY() + 1.0);
	cairo_stroke(cr);
}