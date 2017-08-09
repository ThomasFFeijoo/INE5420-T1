#ifndef INCLUDE_COORDENADAS_HPP_
#define INCLUDE_COORDENADAS_HPP_

class Coordenadas {
public:

	Coordenadas(double xE, double yE, double zE, double auxE) : x(xE), y(yE), z(zE), aux(auxE) {};
	Coordenadas(){}
	~Coordenadas(){};

	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void setAux(double aux);

	double getX();
	double getY();
	double getZ();
	double getAux();

private:
	double x;
	double y;
	double z;
	double aux;
};



#endif /* INCLUDE_COORDENADAS_HPP_ */