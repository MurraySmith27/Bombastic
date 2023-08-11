#ifndef DIRECTIONALLIGHTCOMPONENT_HEADER
#define DIRECTIONALLIGHTCOMPONENT_HEADER

#include "IComponent.h"
#include "Vec3.h"

class DirectionalLightComponent : public IComponent
{
public:

	Vec3 dir;
	Vec3 color;

	DirectionalLightComponent(Vec3 dir, Vec3 color);

	static std::string componentType;
};
#endif
