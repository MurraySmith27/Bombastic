#ifndef ISCRIPT_HEADER
#define ISCRIPT_HEADER
#include <string>
#include "Vec3.h"
/* An interface describing abstract functionality of a script which is attached to an entity.
*/
class IScript {

public:

	IScript();

	IScript(std::string& entityName);

	virtual ~IScript();

	/* Called once per frame.
	*/
	virtual void FrameUpdate(float deltaTime) = 0;

	/* Called when the program starts up.
	*/
	virtual void Start() = 0;

	/* Called on collision between entity1 and entity2 only when one of the two is the entity that has this script attached.
	*/
	virtual void OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) = 0;

protected:

	std::string m_entityName;

};
#endif
