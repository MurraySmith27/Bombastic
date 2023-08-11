#ifndef IMESH_HEADER
#define IMESH_HEADER
#include <string>
#include <vector>
#include "Vec3.h"
#include "Material.h"

class Mesh {
public: 
	float triangleCount;
	float faceCount;

	std::vector<Vec3> vertexPositions;

	std::vector<Vec3> vertexNormals;

	std::vector<std::vector<int>> faceIndices;

	std::vector<std::vector<int>> perFaceVertexNormalIndices;

	std::vector<std::vector<Vec3>> triangles;

	Material mat;


	Mesh(std::string filepath, Material mat);
};

#endif
