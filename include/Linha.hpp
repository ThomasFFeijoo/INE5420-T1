#ifndef INCLUDE_FORMAS_LINHA_HPP_
#define INCLUDE_FORMAS_LINHA_HPP_

#include "Objeto.hpp"
#include <iostream>

class Linha : public Objeto {
public:
	Linha(std::string nomeLinha, std::string tipoLinha, std::vector<Coordenadas> coordenadas) : Objeto(nomeLinha, tipoLinha, coordenadas){}
	~Linha(){}

	void desenhar(cairo_t* cr, std::vector<Coordenadas> coords);

private:
};

#endif /* INCLUDE_FORMAS_LINHA_HPP_ */