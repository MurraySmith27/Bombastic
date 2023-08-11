#ifndef COLLISIONHANDLER_HEADER
#define COLLISIONHANDLER_HEADER
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

class CollisionHandler
{

public:

	//Handles a collision by moving the first entity out of the second so they are no longer colliding.
	static void HandleCollisionMoveOut(TransformComponent* transformToMove, BoxColliderComponent* colliderToMove, TransformComponent* staticTransform, BoxColliderComponent* staticCollider, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld);
};
#endif

