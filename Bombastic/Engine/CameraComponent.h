#ifndef CAMERA_HEADER
#define CAMERA_HEADER
#include "IComponent.h"
#include "Vec3.h"
#include "TransformComponent.h"

class CameraComponent : public IComponent
{

public:
	//Here we set the rule that the camera cannot tilt left/right we would need to store more info if that was possible.
	TransformComponent* target;

	CameraComponent(TransformComponent* target);

	static std::string componentType;

};
#endif

