#ifndef INCLUDE_VIEWPORT_HPP_
#define INCLUDE_VIEWPORT_HPP_

#include <iostream>
#include "DisplayFile.hpp"
#include "Coordenadas.hpp"

class Viewport {
public:
	Viewport();
	Viewport(Coordenadas& inicio, Coordenadas& fim) : coordenadas_minimas(inicio), coordenadas_maximas(fim){}
	~Viewport(){}

	void transformada(cairo_t* cr, Coordenadas inicioDaWindow, Coordenadas fimDaWindow, DisplayFile* displayfile);
private:
	Coordenadas coordenadas_minimas;	//Xmin, Ymin, Zmin
	Coordenadas coordenadas_maximas;	//Xmax, Ymax, Zmax
	Coordenadas calcCoordTransf(Coordenadas inicioDaWindow, Coordenadas fimDaWindow, Coordenadas coordenadas_objeto);
};

#endif /* INCLUDE_VIEWPORT_HPP_ */