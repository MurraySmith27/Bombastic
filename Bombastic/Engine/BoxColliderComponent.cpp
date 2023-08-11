#include "stdafx.h"
#include "BoxColliderComponent.h"
#include <math.h>


BoxColliderComponent::BoxColliderComponent(Vec3 center, Vec3 dimensions, CollisionLayers layer) : center(center), dimensions(dimensions), layer(layer) {
	this->componentType = "box_collider";
}

void BoxColliderComponent::setLayer(CollisionLayers newLayer) {
	this->layer = newLayer;
}

bool BoxColliderComponent::isColliding(BoxColliderComponent* other, TransformComponent* thisTransform, TransformComponent* otherTransform, Vec3& overlapCenter, Vec3& overlapDimensions) {
	Vec4 centerWorld1 = thisTransform->getWorldSpaceCoords(Vec4(this->center[0], this->center[1], this->center[2], 1));
	centerWorld1 = centerWorld1 / centerWorld1[3];
	Vec4 dimensionsWorld1 = thisTransform->getWorldSpaceCoords(Vec4(this->dimensions[0], this->dimensions[1], this->dimensions[2], 0)); //double check this
	dimensionsWorld1 = Vec4(abs(dimensionsWorld1[0]), abs(dimensionsWorld1[1]), abs(dimensionsWorld1[2]), 0) * 2;
	Vec3 min1 = centerWorld1 - dimensionsWorld1 / 2;
	Vec3 max1 = min1 + dimensionsWorld1;

	Vec4 centerWorld2 = otherTransform->getWorldSpaceCoords(Vec4(other->center[0], other->center[1], other->center[2], 1));
	centerWorld2 = centerWorld2 / centerWorld2[3];
	Vec4 dimensionsWorld2 = otherTransform->getWorldSpaceCoords(Vec4(other->dimensions[0], other->dimensions[1], other->dimensions[2], 0)); //double check this
	dimensionsWorld2 = Vec4(abs(dimensionsWorld2[0]), abs(dimensionsWorld2[1]), abs(dimensionsWorld2[2]), 0) * 2;
	Vec3 min2 = centerWorld2 - dimensionsWorld2 / 2;
	Vec3 max2 = min2 + dimensionsWorld2;

	if ((min1[0] <= max2[0]) && (max1[0] >= min2[0]) &&
		(min1[1] <= max2[1]) && (max1[1] >= min2[1]) &&
		(min1[2] <= max2[2]) && (max1[2] >= min2[2])
		) {
		//collision found. compute overlapping box
		Vec3 a = max2 - min1;
		Vec3 b = max1 - min2;
		Vec3 minOfMax = Vec3(std::min(max1[0], max2[0]), std::min(max1[1], max2[1]), std::min(max1[2], max2[2]));
		Vec3 maxOfMin = Vec3(std::max(min1[0], min2[0]), std::max(min1[1], min2[1]), std::max(min1[2], min2[2]));
		overlapDimensions = minOfMax - maxOfMin;

		overlapCenter[0] = overlapDimensions[0] / 2 + maxOfMin[0];
		overlapCenter[1] = overlapDimensions[1] / 2 + maxOfMin[1];
		overlapCenter[2] = overlapDimensions[2] / 2 + maxOfMin[2];
		return true;
	}
	return false;
}


std::string BoxColliderComponent::componentType;