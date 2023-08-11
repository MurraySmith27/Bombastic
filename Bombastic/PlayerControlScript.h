#ifndef MONKEYTESTSCRIPT_HEADER
#define MONKEYTESTSCRIPT_HEADER
#include "Engine/IScript.h"
#include <string>
#include "Engine/TransformComponent.h"
class PlayerControlScript : public IScript 
{

private:

	bool playerHasBomb = false;

	std::string bombName;
	
	bool levelComplete = false;

	float lookHorizontalSpeed = 19;
	float lookVerticalSpeed = 5;

	float moveSpeed = 10;
	float stickInputThreshhold = 0.5f;

	TransformComponent* bombTransform;

	void HandleMoveAndLook(float deltaTime);

	void CheckForShootEvent();
public:

	PlayerControlScript(std::string entityName);

	void FrameUpdate(float deltaTime);

	void Start();

	void OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) override;
};
#endif
