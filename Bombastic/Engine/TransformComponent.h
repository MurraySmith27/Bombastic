#ifndef TRANSFORMCOMPONENT_HEADER
#define TRANSFORMCOMPONENT_HEADER
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4x4.h"
#include <string>
#include "IComponent.h"

class TransformComponent : public IComponent
{


public:

	Mat4x4 transform;

	TransformComponent* parent;

	void scaleBy(float x, float y, float z);

	void scaleBy(Vec3 scaleFactor);

	void rotateBy(float angleX, float angleY, float angleZ);

	void rotateBy(Vec3 angles);

	void moveBy(float x, float y, float z);

	void moveBy(Vec3 moveVec);

	void moveTo(float x, float y, float z);

	void moveTo(Vec3 moveVec);

	Vec4 getWorldSpaceCoords(Vec4 localCoords);

	bool getWorldSpaceNormal (Vec4 localNormal, Vec4& result);

	bool getLocalSpaceCoords(Vec4 worldCoords, Vec4& localCoords);

	static Mat4x4 getRotationMatrix(float angleX, float angleY, float angleZ);

	TransformComponent();

	TransformComponent(Vec3 position);

	TransformComponent(Vec3 position, TransformComponent* parent);

	static std::string componentType;

};
#endif
