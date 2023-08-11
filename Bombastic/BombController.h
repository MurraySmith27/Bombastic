#ifndef BOMBCONTROLLER_HEADER
#define BOMBCONTROLLER_HEADER
#include "Engine/IScript.h"
#include <string>
#include "Engine/Vec3.h"
class BombController : public IScript
{

private:

	float rotationSpeed = 15;

	bool isBeingThrown;
	float throwSpeed = 0.5f;
	float maxThrowTimeSeconds = 10;
	float throwCounter = 0;
	Vec3 throwDirection;

	bool isFuseBurning;
	float fuseBurningCounter = 0;
	float fuseBurningTimeSeconds = 3;
	float fuseBurningColorAlternateTimeSeconds = 0.5;
	float timeLastSwitchedColors = 0;
	bool fuseBurningIsRed = true;

	Vec3 fuseBurningRedDiffuse = Vec3(0.8, 0.2, 0.2);
	Vec3 fuseBurningRedAmbient = Vec3(0.4, 0.2, 0.2);
	Vec3 fuseBurningWhiteDiffuse = Vec3(1, 0.6, 0.6);
	Vec3 fuseBurningWhiteAmbient = Vec3(0.6, 0.3, 0.3);

	bool isExploding;
	Vec3 explodingColliderDimensions = Vec3(1, 1, 1);
	float explodingCounter = 0;
	float explodingTimeSeconds = 1;

public:

	BombController(std::string entityName);

	void FrameUpdate(float deltaTime);

	void Start();

	void OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) override;

	void BeginThrow(Vec3 worldDirection);
};
#endif
