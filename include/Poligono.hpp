#ifndef INCLUDE_POLIGONO_HPP_
#define INCLUDE_POLIGONO_HPP_

#include "Objeto.hpp"
#include <iostream>

class Poligono : public Objeto {
private:
	std::vector<Coordenadas> coordenadas; //vertices

public:

	Poligono(std::string nomePoligono, std::string tipoPoligono, std::vector<Coordenadas> coordenadas) : Objeto(nomePoligono, tipoPoligono, coordenadas){
	}

	~Poligono(){}

	void desenhar(cairo_t* surf, std::vector<Coordenadas> coords);
};
#endif /* INCLUDE_POLIGONO_HPP_ */