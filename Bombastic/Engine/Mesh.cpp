#include "stdafx.h"
#include "Mesh.h";

#include <fstream>
#include <sstream>
#include <iostream>


Mesh::Mesh(std::string filePath, Material mat) : mat(mat) {

	this->vertexPositions = std::vector<Vec3>();

	this->faceIndices = std::vector<std::vector<int>>();

	//Here is a simple implementation of a .obj loader.

	std::ifstream objFile;

	try {
		objFile.open(filePath);
	}
	catch (void* e) {
		std::cerr << "Could not open obj file at path: " << filePath << "!" << std::endl;
		exit(-1);
	}




	std::string line;
	while (std::getline(objFile, line)) {

		std::istringstream lineStream(line);

		std::string firstPart;
		lineStream >> firstPart;
		if (firstPart == "v") {
			std::string x, y, z;
			lineStream >> x;
			lineStream >> y;
			lineStream >> z;
			this->vertexPositions.push_back(Vec3(std::stof(x), std::stof(y), std::stof(z)));

		}
		else if (firstPart == "f") {
			std::string a, b, c;
			std::string anorm, bnorm, cnorm;
			std::string auv, buv, cuv;
			lineStream >> a;
			auv = a.substr(a.find("/") + 1);
			anorm = auv.substr(auv.find("/") + 1);

			lineStream >> b;
			buv = b.substr(b.find("/") + 1);
			bnorm = buv.substr(buv.find("/") + 1);

			lineStream >> c;
			cuv = c.substr(c.find("/") + 1);
			cnorm = cuv.substr(cuv.find("/") + 1);


			this->faceIndices.push_back({ std::stoi(a), std::stoi(b), std::stoi(c) });
			if (anorm != "") {
				this->perFaceVertexNormalIndices.push_back({ std::stoi(anorm), std::stoi(bnorm), std::stoi(cnorm) });
			}
		}
		else if (firstPart == "vn") {
			std::string x, y, z;
			lineStream >> x;
			lineStream >> y;
			lineStream >> z;
			this->vertexNormals.push_back(Vec3(std::stof(x), std::stof(y), std::stof(z)));
		}
	}

	//construct triangles
	for (int i = 0; i < faceIndices.size(); i++) {
		std::vector<Vec3> triangle(3);

		triangle[0] = vertexPositions[faceIndices[i][0] - 1];
		triangle[1] = vertexPositions[faceIndices[i][1] - 1];
		triangle[2] = vertexPositions[faceIndices[i][2] - 1];
		triangles.push_back(triangle);
	}
}