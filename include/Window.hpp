#ifndef INCLUDE_WINDOW_HPP_
#define INCLUDE_WINDOW_HPP_

#include <iostream>
#include "DisplayFile.hpp"
#include "Coordenadas.hpp"

class Window {
public:
	Window(Coordenadas* inicio, Coordenadas* fim, DisplayFile * world) : inicioDaWindow(inicio), fimDaWindow(fim), displayFile(world){}
	~Window(){}

	void zoom(double porcentagem);
	void mover(double x, double y, double z);
	void setCoordenadas(Coordenadas* inicio, Coordenadas* fim){inicioDaWindow = inicio; fimDaWindow = fim;}
	Coordenadas* getInicioDaWindow(){return inicioDaWindow;}
	Coordenadas* getFimDaWindow(){return fimDaWindow;}

	//mexer nas Coordenadas zoom
private:
	Coordenadas * inicioDaWindow;
	Coordenadas * fimDaWindow;
};



#endif /* INCLUDE_WINDOW_HPP_ */