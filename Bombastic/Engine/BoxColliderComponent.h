#ifndef BOXCOLLIDERCOMPONENT_HEADER
#define BOXCOLLIDERCOMPONENT_HEADER

#include "IEntity.h"
#include "Vec3.h"
#include "IComponent.h"
#include "TransformComponent.h"

class BoxColliderComponent : public IComponent
{
public:
	static enum CollisionLayers {
		Impassable,
		BombSpawner,
		Exploding,
		Destructable,
		Goal,
		None
	};


	Vec3 center;
	Vec3 dimensions;

	CollisionLayers layer = CollisionLayers::None;

	BoxColliderComponent(Vec3 center, Vec3 dimensions, CollisionLayers layer = CollisionLayers::None);

	bool isColliding(BoxColliderComponent* other, TransformComponent* thisTransform, TransformComponent* otherTransform, Vec3& overlapCenter, Vec3& overlapDimensions);

	void setLayer(CollisionLayers newLayer);

	static std::string componentType;
};
#endif

