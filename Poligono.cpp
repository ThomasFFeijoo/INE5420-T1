#include "Poligono.hpp"


void Poligono::desenhar(cairo_t* cr, std::vector<Coordenadas> coords){
	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, coords[0].getX(), coords[0].getY());
	for (auto coord : coords)
		cairo_line_to(cr, coord.getX(), coord.getY());
	cairo_stroke(cr);
}