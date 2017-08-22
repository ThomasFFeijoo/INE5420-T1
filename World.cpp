#include "World.hpp"

bool World::adicionaObjetosNoMundo(Objeto* obj){
	return world_displayFile.addObjectInTheWorld(obj);
}

void World::removeObjetosNoMundo(std::string nome){
	world_displayFile.removeObjectFromTheWorld(nome);
}

void World::destroiTodosObjetosDoMundo(){
	for (auto obj : world_displayFile.getAllObjectsFromTheWorld()){
		world_displayFile.removeObjectFromTheWorld((obj.second)->getName());
	}
}

void World::transformarObjeto(std::string nome, Matriz::Matriz<double> matriz){
	world_displayFile.getTheObjectFromTheWorld(nome)->transformaObjeto(matriz);
}

void World::scalonarObjeto(std::string nome, Matriz::Matriz<double> matriz){
	Objeto* ob = world_displayFile.getTheObjectFromTheWorld(nome);
	std::vector<double> tmp0 = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	Matriz::Matriz<double> tmp1 = Matriz::Matriz<double>(3u,3u,tmp0);
	tmp1(2,0) = -ob->centroDoObjeto().getX();
	tmp1(2,1) = -ob->centroDoObjeto().getY();
	Matriz::Matriz<double> tmp2 = Matriz::Matriz<double>(3u,3u,tmp0);
	tmp2(2,0) = ob->centroDoObjeto().getX();
	tmp2(2,1) = ob->centroDoObjeto().getY();
	Matriz::Matriz<double> tmp = tmp1 * matriz;
	Matriz::Matriz<double> tmp3 = tmp * tmp2;
	ob->transformaObjeto(tmp3);
}

void World::rotacionarObjeto(std::string nome, bool coordArbitraria, Coordenadas coords, Matriz::Matriz<double> matriz){
	std::vector<double> tmp0 = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	Matriz::Matriz<double> ida = Matriz::Matriz<double>(3u,3u,tmp0);
	Matriz::Matriz<double> volta = Matriz::Matriz<double>(3u,3u,tmp0);
	if (coordArbitraria){
		ida(2,0) = -coords.getX();
		volta(2,0) = coords.getX();
		ida(2,1) = -coords.getY();
		volta(2,1) = coords.getY();
	} else {
		Coordenadas cd = world_displayFile.getTheObjectFromTheWorld(nome)->centroDoObjeto();
		ida(2,0) = -cd.getX();
		volta(2,0) = cd.getX();
		ida(2,1) = -cd.getY();
		volta(2,1) = cd.getY();
	}
	Matriz::Matriz<double> tmp = ida * matriz;
	Matriz::Matriz<double> tmp3 = tmp * volta;
	world_displayFile.getTheObjectFromTheWorld(nome)->transformaObjeto(tmp3);
}
