#include "stdafx.h"
#include "TransformComponent.h"
#include "Vec4.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

TransformComponent::TransformComponent() {
	this->componentType = "transform";
	this->transform = Mat4x4(
		Vec4(1, 0, 0, 0),
		Vec4(0, 1, 0, 0),
		Vec4(0, 0, 1, 0),
		Vec4(0, 0, 0, 1)
	);
}

TransformComponent::TransformComponent(Vec3 pos) {
	this->componentType = "transform";

	this->transform = Mat4x4(
		Vec4(1, 0, 0, pos[0]),
		Vec4(0, 1, 0, pos[1]),
		Vec4(0, 0, 1, pos[2]),
		Vec4(0, 0, 0, 1)
	);
}

TransformComponent::TransformComponent(Vec3 pos, TransformComponent* parent) {
	this->componentType = "transform";

	this->transform = Mat4x4(
		Vec4(1, 0, 0, pos[0]),
		Vec4(0, 1, 0, pos[1]),
		Vec4(0, 0, 1, pos[2]),
		Vec4(0, 0, 0, 1)
	);

	this->parent = parent;
}

void TransformComponent::scaleBy(float x, float y, float z) {

	this->transform[0][0] = this->transform[0][0] * x;
	this->transform[1][0] = this->transform[1][0] * x;
	this->transform[2][0] = this->transform[2][0] * x;
	this->transform[3][0] = this->transform[3][0] * x;

	this->transform[0][1] = this->transform[0][1] * y;
	this->transform[1][1] = this->transform[1][1] * y;
	this->transform[2][1] = this->transform[2][1] * y;
	this->transform[3][1] = this->transform[3][1] * y;

	this->transform[0][2] = this->transform[0][2] * z;
	this->transform[1][2] = this->transform[1][2] * z;
	this->transform[2][2] = this->transform[2][2] * z;
	this->transform[3][2] = this->transform[3][2] * z;
}

void TransformComponent::scaleBy(Vec3 scaleFactor) {
	this->scaleBy(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
}

Mat4x4 TransformComponent::getRotationMatrix(float angleX, float angleY, float angleZ) {
	float x = angleX * M_PI / 180;
	float y = angleY * M_PI / 180;
	float z = angleZ * M_PI / 180;

	Mat4x4 rotation= Mat4x4(
		Vec4(1, 0, 0, 0),
		Vec4(0, cos(x), -sin(x), 0),
		Vec4(0, sin(x), cos(x), 0),
		Vec4(0, 0, 0, 1)
	);

	rotation = rotation * Mat4x4(
		Vec4(cos(y), 0, sin(y), 0),
		Vec4(0, 1, 0, 0),
		Vec4(-sin(y), 0, cos(y), 0),
		Vec4(0, 0, 0, 1)
	);

	rotation = rotation * Mat4x4(
		Vec4(cos(z), -sin(z), 0, 0),
		Vec4(sin(z), cos(z), 0, 0),
		Vec4(0, 0, 1, 0),
		Vec4(0, 0, 0, 1)
	);
	return rotation;
}

//Rotates the transform around the axes specified using euler angles.
void TransformComponent::rotateBy(float angleX, float angleY, float angleZ) {
	transform = transform * TransformComponent::getRotationMatrix(angleX, angleY, angleZ);
}

void TransformComponent::rotateBy(Vec3 angles) {
	this->rotateBy(angles[0], angles[1], angles[2]);
}

void TransformComponent::moveBy(float x, float y, float z) {
	transform[0][3] = transform[0][3] + x;
	transform[1][3] = transform[1][3] + y;
	transform[2][3] = transform[2][3] + z;
}

void TransformComponent::moveBy(Vec3 moveVec) {
	this->moveBy(moveVec[0], moveVec[1], moveVec[2]);
}

void TransformComponent::moveTo(float x, float y, float z) {


	transform[0][3] = x;
	transform[1][3] = y;
	transform[2][3] = z;
}

void TransformComponent::moveTo(Vec3 moveVec) {
	this->moveTo(moveVec[0], moveVec[1], moveVec[2]);
}

Vec4 TransformComponent::getWorldSpaceCoords(Vec4 localCoords) {
	TransformComponent* transform = this;
	Vec4 worldPos = localCoords;
	while (transform != NULL) {
		worldPos = (transform->transform * worldPos);
		transform = transform->parent;
	}

	return worldPos;
}

bool TransformComponent::getWorldSpaceNormal(Vec4 localNormal, Vec4& worldNormal) {
	TransformComponent* transform = this;
	worldNormal= localNormal;
	while (transform != NULL) {
		Mat4x4 inverse;
		bool inverseExists = transform->transform.inverse(inverse);
		if (!inverseExists) {
			return false;
		}
		worldNormal = (inverse.transpose() * worldNormal);
		transform = transform->parent;
	}
}

bool TransformComponent::getLocalSpaceCoords(Vec4 worldCoords, Vec4& localCoords) {
	std::vector<Mat4x4> inverses;
	Vec4 translation = Vec4(this->transform[0][3], this->transform[1][3], this->transform[2][3], 1);

	TransformComponent* transform = this;
	while (transform != NULL) {
		Mat4x4 inverse;
		bool inverseExists = transform->transform.inverse(inverse);
		if (!inverseExists) {
			return false;
		}
		inverses.push_back(inverse);
		transform = transform->parent;
	}

	localCoords = worldCoords;
	for (int i = inverses.size() - 1; i >= 0; i--) {
		localCoords = inverses[i] * localCoords;
	}

	return true;
}

std::string TransformComponent::componentType;