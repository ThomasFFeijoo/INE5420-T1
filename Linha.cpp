#include "Linha.hpp"


void Linha::desenhar(cairo_t* cr, std::vector<Coordenadas> coords){
	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, coords[0].getX(), coords[0].getY());
	cairo_line_to (cr, coords[1].getX(), coords[1].getY());
	cairo_stroke(cr);
}