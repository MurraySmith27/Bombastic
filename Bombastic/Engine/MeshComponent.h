#ifndef MESHCOMPONENT_HEADER
#define MESHCOMPONENT_HEADER

#include "Material.h"
#include "Mesh.h"
#include "IComponent.h"

class MeshComponent : public IComponent
{

public:
	Mesh mesh;


	MeshComponent(Mesh mesh);

	static std::string componentType;

};

#endif