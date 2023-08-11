#ifndef ENTITYNAMES_HEADER
#define ENTITYNAMES_HEADER
#include <string>

class EntityData {
public:
	static std::string cameraEntityName;
	static std::string cameraFollowTargetName;

	static std::string wallName;
	static std::string bottomWallName;

	static std::string bombSpawner;
	static std::string bombName;

	static std::string playerName;

	static std::string goalName;
	static std::string goalTextName;

	

	static int entityIdGenerator;
};
#endif