#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include <iostream>
#include <unordered_map>
#include "Objeto.hpp"

class DisplayFile {
private:
	std::unordered_map<std::string, Objeto*> objectsInTheWorld;
public:
	DisplayFile(){};
	~DisplayFile(){};

	bool addObjectInTheWorld(Objeto* obj);
	void removeObjectFromTheWorld(std::string objName);
	Objeto* getTheObjectFromTheWorld(std::string objName);
	std::unordered_map<std::string, Objeto* > getAllObjectsFromTheWorld();
	bool isEmpty();
};

#endif