#ifndef BOMBSPAWNERCONTROLLER_HEADER
#define BOMBSPAWNERCONTROLLER_HEADER
#include "Engine/IScript.h"
#include <string>
class BombSpawnerController : public IScript
{

private:

	float counter = 0;

	float bombSpawnCooldownSeconds = 5;

	std::string bombName;


	void SpawnBomb();

public:
	bool hasBomb = false;

	BombSpawnerController();

	BombSpawnerController(std::string entityName);

	void FrameUpdate(float deltaTime);

	void Start();

	void OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) override;

	std::string getBombName();
};
#endif