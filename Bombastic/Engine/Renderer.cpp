#include "stdafx.h"
#include "Renderer.h"

#include "../app/app.h"
#include <iostream>

Renderer::Renderer() {
}


Vec3 Renderer::BlinnPhong(Vec3 eyeToVertex, Vec3 normal, Material mat, std::vector<DirectionalLightComponent> directionalLights) {

	//compute shading contributions to get color.
	//note that all these calculations are done in world space, which shouldnt matter.

	Vec3 color = mat.ambientColor * mat.ambientCoeff;

	for (int light_num = 0; light_num < directionalLights.size(); light_num++) {

		Vec3 light_dir = -directionalLights[light_num].dir;

		Vec3 h = (light_dir.normalize() - eyeToVertex).normalize();

		//add diffuse contribution
		color = color + (mat.diffuseColor * directionalLights[light_num].color) * max(0, Vec3(normal).dot(light_dir));

		//add specular contribution
		color = color + (mat.specularColor * directionalLights[light_num].color) * std::pow(max(0, Vec3(normal).dot(h)), mat.phong_exp);
	}

	return color;
}



void Renderer::DrawMesh(MeshComponent meshComponent, TransformComponent transformComponent, CameraComponent camera, TransformComponent cameraTransform, std::vector<DirectionalLightComponent> directionalLights) {

	// we translate eyePos and targetPos to world space.
	Vec4 eyePos = cameraTransform.getWorldSpaceCoords(Vec4(0, 0, 0, 1));
	eyePos = eyePos / eyePos[3];
	Vec4 targetPos = camera.target->getWorldSpaceCoords(Vec4(0, 0, 0, 1));
	targetPos = targetPos / targetPos[3];
	Vec3 diff(targetPos - eyePos);
	diff = diff.normalize();
	Vec4 targetDir = Vec4(diff[0], diff[1], diff[2], 1);
	Vec3 cameraRight = Vec3(0, 1, 0).cross(Vec3(targetDir));

	Vec3 targetDirVec3(targetDir);
	Vec3 cameraUp = targetDirVec3.cross(cameraRight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 1, 30);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyePos[0], eyePos[1], eyePos[2],
		targetPos[0], targetPos[1], targetPos[2],
		cameraUp[0], cameraUp[1], cameraUp[2]);

	std::vector<std::vector<int>>* faces = &meshComponent.mesh.faceIndices;
	std::vector<Vec3>* vertices = &meshComponent.mesh.vertexPositions;
	std::vector<Vec3>* vertexNormals = &meshComponent.mesh.vertexNormals;
	std::vector<std::vector<int>>* perFaceVertexNormalIndices = &meshComponent.mesh.perFaceVertexNormalIndices;

	for (int i = 0; i < faces->size(); i++) {
		
		glBegin(GL_TRIANGLES);

		std::vector<Vec4> vertexPositions;
		for (int j = 0; j < 3; j++) {
			vertexPositions.push_back(Vec4((*vertices)[(*faces)[i][j] - 1][0], (*vertices)[(*faces)[i][j] - 1][1], (*vertices)[(*faces)[i][j] - 1][2], 1));
		}

		std::vector<DirectionalLightComponent> localSpaceDirectionalLights;
		for (int j = 0; j < directionalLights.size(); j++) {
			Vec4 localLightDir;
			transformComponent.getLocalSpaceCoords(Vec4(directionalLights[j].dir[0], directionalLights[j].dir[1], directionalLights[j].dir[2], 0), localLightDir);
			localSpaceDirectionalLights.push_back(DirectionalLightComponent(localLightDir, directionalLights[j].color));
		}
		
		for (int j = 0; j < 3; j++) {
			std::vector<int> face = (*faces)[i];
			Vec4 vertexPos = vertexPositions[j];
			
			std::vector<int> faceNormalIndices = (*perFaceVertexNormalIndices)[i];
			Vec4 vertexNormal = Vec4((*vertexNormals)[faceNormalIndices[j] - 1][0], (*vertexNormals)[faceNormalIndices[j] - 1][1], (*vertexNormals)[faceNormalIndices[j] - 1][2], 1);
			//Mat4x4 inverse;
			
			//we need to multiply the normal by the inverse transpose of the transformation.
			/*Vec4 worldVertexNormal;
			bool inverseExists = transformComponent.getWorldSpaceNormal(vertexNormal, worldVertexNormal);
			worldVertexNormal = worldVertexNormal / worldVertexNormal[3];
			
			if (!inverseExists) {
				std::cerr << "There is no inverse for the transformation to apply to the normals! This means that the transformation is not affine." << std::endl;
				exit(-1);
			}*/

			Vec4 worldVertexPos = transformComponent.getWorldSpaceCoords(vertexPos);

			worldVertexPos = worldVertexPos / worldVertexPos[3];

			Vec3 eyeToVertex = Vec3(vertexPos).normalize();
			
			//we do this in world space. Assuming lights are always defined in world space.
			Vec3 color = BlinnPhong(eyeToVertex, vertexNormal, meshComponent.mesh.mat, directionalLights);
			
			glColor3f(color[0], color[1], color[2]);
			glVertex3f(worldVertexPos[0], worldVertexPos[1], worldVertexPos[2]);
		}
		glEnd();
	}
}