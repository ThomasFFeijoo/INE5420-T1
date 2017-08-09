#include "Window.hpp"

void Window::zoom(double porcentagem){
	double larguraDaWindow = fimDaWindow->getX() - inicioDaWindow->getX();
	double alturaDaWindow = fimDaWindow->getY() - inicioDaWindow->getY();
	double centroDaWindowX = (larguraDaWindow/2);
	double centroDaWindowY = (alturaDaWindow/2);
	larguraDaWindow = larguraDaWindow / porcentagem;
	alturaDaWindow = alturaDaWindow / porcentagem;
	inicioDaWindow->setX((centroDaWindowX - larguraDaWindow)/2);
	inicioDaWindow->setY((centroDaWindowY - alturaDaWindow)/2);
	fimDaWindow->setX((centroDaWindowX + larguraDaWindow)/2);
	fimDaWindow->setY((centroDaWindowY + alturaDaWindow)/2);
}

void Window::mover(double x, double y, double z){
	inicioDaWindow->setX(inicioDaWindow->getX()+x);
	inicioDaWindow->setY(inicioDaWindow->getY()+y);
	fimDaWindow->setX(fimDaWindow->getX()+x);
	fimDaWindow->setY(fimDaWindow->getY()+y);
}
