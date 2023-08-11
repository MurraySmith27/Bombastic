#include "stdafx.h"
#include "CollisionHandler.h"


void CollisionHandler::HandleCollisionMoveOut(TransformComponent* transformToMove, BoxColliderComponent* colliderToMove, TransformComponent* staticTransform, BoxColliderComponent* staticCollider, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) {
	//greedy approach. march in each direction, If the area of the overlap is reduced, march from there. Return when out.

	Vec3 directions[] = {
		Vec3(0,0,1),
		Vec3(0,1,0),
		Vec3(1,0,0),
		Vec3(0,0,-1),
		Vec3(0,-1,0),
		Vec3(-1,0,0)
	};

	int currDirection = 0;
	float currentOverlapVolume = overlapDimensionsWorld[0] * overlapDimensionsWorld[1] * overlapDimensionsWorld[2];
	if (currentOverlapVolume == 0) return;
	Vec3 overlapCenter, overlapDimensions;


	//initial step
	transformToMove->moveBy(directions[currDirection][0], directions[currDirection][1], directions[currDirection][2]);

	while (colliderToMove->isColliding(staticCollider, transformToMove, staticTransform, overlapCenter, overlapDimensions)) {
		float overlapVolume = overlapDimensions[0] * overlapDimensions[1] * overlapDimensions[2];
		if (overlapVolume == 0) {
			break;
		}
		else if (abs(overlapVolume) < abs(currentOverlapVolume)) {
			currentOverlapVolume = overlapVolume;
		}
		else {
			//reverse the march.
			transformToMove->moveBy(-directions[currDirection][0], -directions[currDirection][1], -directions[currDirection][2]);
			currDirection = (currDirection + 1) % 6;
			//march in a new direction
		}
		transformToMove->moveBy(directions[currDirection][0], directions[currDirection][1], directions[currDirection][2]);
	}

	//now that we've found a direction, tiptoe back until just before colliding again
	float tiptoeLength = 0.1f;
	while (!colliderToMove->isColliding(staticCollider, transformToMove, staticTransform, overlapCenter, overlapDimensions)) {

		transformToMove->moveBy(-directions[currDirection][0] * tiptoeLength, -directions[currDirection][1] * tiptoeLength, -directions[currDirection][2] * tiptoeLength);
	}
	transformToMove->moveBy(directions[currDirection][0] * tiptoeLength, directions[currDirection][1] * tiptoeLength, directions[currDirection][2] * tiptoeLength);
}