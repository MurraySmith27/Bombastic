#include "stdafx.h"
#include "MeshComponent.h"


MeshComponent::MeshComponent(Mesh mesh) : mesh(mesh) {
	this->componentType = "mesh";
}

std::string MeshComponent::componentType;