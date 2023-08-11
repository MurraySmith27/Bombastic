#include "stdafx.h"
#include "DirectionalLightComponent.h"



DirectionalLightComponent::DirectionalLightComponent(Vec3 dir, Vec3 color) : dir(dir), color(color) {
	this->componentType = "directional_light";
}

std::string DirectionalLightComponent::componentType;