#include "stdafx.h"
#include "CameraComponent.h"


CameraComponent::CameraComponent(TransformComponent* target) {
	this->componentType = "camera";
	this->target = target;
}

std::string CameraComponent::componentType;