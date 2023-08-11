#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "MeshComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "TextComponent.h"

class Renderer
{


private:

	Vec3 BlinnPhong(Vec3 eyeToVertex, Vec3 normal, Material mat, std::vector<DirectionalLightComponent> directionalLights);

public:
	Renderer();

	void DrawMesh(MeshComponent meshComponent, TransformComponent transformComponent, CameraComponent camera, TransformComponent cameraTransform, std::vector<DirectionalLightComponent> directionalLight);

};
#endif

