#include "stdafx.h"
#include "MapLoader.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::vector<char>> MapLoader::LoadMap(std::string filePath) {

	std::ifstream mapFile;

	try {
		mapFile.open(filePath);
	}
	catch (void* e) {
		std::cerr << "Could not open txt file at path: " << filePath << "!" << std::endl;
		exit(-1);
	}

	std::vector<std::vector<char>> mapData;

	std::string line;
	while (std::getline(mapFile, line)) {

		std::istringstream lineStream(line);

		std::string nextPart = "blah";
		std::vector<char> thisRow;
		while (nextPart != "") {
			nextPart = "";
			lineStream >> nextPart;
			thisRow.push_back(nextPart[0]);
		}
		mapData.push_back(thisRow);
	}

	return mapData;

}
