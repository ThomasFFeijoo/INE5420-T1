#ifndef INCLUDE_WORLD_HPP_
#define INCLUDE_WORLD_HPP_

#include "DisplayFile.hpp"
#include "Matriz.hpp"

class World {
private:
	DisplayFile world_displayFile;
public:
	World(){}
	~World(){}
	bool adicionaObjetosNoMundo(Objeto* obj);
	void removeObjetosNoMundo(std::string nome);
	void destroiTodosObjetosDoMundo();
	DisplayFile* getDisplayfile(){return &world_displayFile;}
	void transformarObjeto(std::string nome, Matriz::Matriz<double> matriz);
	void scalonarObjeto(std::string nome, Matriz::Matriz<double> matriz);
	void rotacionarObjeto(std::string nome, bool coordArbitraria, Coordenadas coords, Matriz::Matriz<double> matriz);
};
#endif /* INCLUDE_WORLD_HPP_ */