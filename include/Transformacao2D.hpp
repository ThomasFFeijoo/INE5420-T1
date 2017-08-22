#ifndef INCLUDE_TRANSFORMACAO2D_HPP_
#define INCLUDE_TRANSFORMACAO2D_HPP_

#include <iostream>
#include "Coordenadas.hpp"
#include <cmath>
#include <vector>
#include "Matriz.hpp"

class Transformacao2D {
public:
	Transformacao2D(){}
	~Transformacao2D(){};
	Matriz::Matriz<double> translacao(double dx, double dy);
	Matriz::Matriz<double> escalonamento(double sx, double sy);
	Matriz::Matriz<double> rotacao(double graus);
};
#endif /* INCLUDE_TRANSFORMACAO2D_HPP_ */
