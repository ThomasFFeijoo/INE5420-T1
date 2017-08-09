#ifndef INCLUDE_FORMAS_PONTO_HPP_
#define INCLUDE_FORMAS_PONTO_HPP_

#include "Objeto.hpp"
#include <gtk/gtk.h>

class Ponto : public Objeto {
public:
	Ponto(std::string nomePonto, std::string tipoPonto, std::vector<Coordenadas> coordenadas) : Objeto(nomePonto, tipoPonto, coordenadas){}

	~Ponto(){}
	void desenhar(cairo_t* surf, std::vector<Coordenadas> coords);

private:
};


#endif /* INCLUDE_FORMAS_PONTO_HPP_ */