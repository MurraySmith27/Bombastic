#include "stdafx.h"
#include "BombController.h"
#include "Engine/EntityManager.h"
#include "Engine/TransformComponent.h"
#include "Engine/BoxColliderComponent.h"
#include "Engine/MeshComponent.h";
#include "App/app.h"

BombController::BombController(std::string entityName) : IScript(entityName) {

}


void BombController::Start() {

}

void BombController::FrameUpdate(float deltaTime) {

	TransformComponent* thisTransform = EntityManager::GetEntityByName(this->m_entityName)->GetComponent<TransformComponent>();


	if (isBeingThrown) {
		thisTransform->moveBy(throwDirection.normalize() * throwSpeed);

		if (throwCounter > maxThrowTimeSeconds) {
			//been in the air too long. Destroy
			EntityManager::DeregisterEntity(m_entityName);
		}

		throwCounter += deltaTime / 1000;
	}
	else if (isFuseBurning) {
		if (fuseBurningCounter > fuseBurningTimeSeconds) {
			//explode!
			
			fuseBurningCounter = 0;
			timeLastSwitchedColors = 0;
			isFuseBurning = false;
			App::PlaySound(".\\TestData\\Test.wav");
			isExploding = true;
			BoxColliderComponent* bombCollider = EntityManager::GetEntityByName(m_entityName)->GetComponent<BoxColliderComponent>();

			bombCollider->dimensions = explodingColliderDimensions;
			bombCollider->layer = BoxColliderComponent::CollisionLayers::Exploding;
			//swap out the mesh for an explosion mesh
			MeshComponent* bombMesh = EntityManager::GetEntityByName(m_entityName)->GetComponent<MeshComponent>();
			bombMesh->mesh = Mesh(".\\Meshes\\Simple.obj", Material(fuseBurningRedAmbient, 0.5, fuseBurningRedDiffuse, Vec3(1,1,1), 10));
			
		}
		else {
			if (fuseBurningCounter - timeLastSwitchedColors > fuseBurningColorAlternateTimeSeconds) {
				timeLastSwitchedColors = fuseBurningCounter;
				MeshComponent* bombMesh = EntityManager::instance.GetEntityByName(this->m_entityName)->GetComponent<MeshComponent>();
				if (fuseBurningIsRed) {
					bombMesh->mesh.mat.diffuseColor = fuseBurningWhiteDiffuse;
					bombMesh->mesh.mat.ambientColor = fuseBurningWhiteAmbient;
					fuseBurningIsRed = false;
				}
				else {
					bombMesh->mesh.mat.diffuseColor = fuseBurningRedDiffuse;
					bombMesh->mesh.mat.ambientColor = fuseBurningRedAmbient;
					fuseBurningIsRed = true;
				}
			}
			fuseBurningCounter += deltaTime / 1000;
		}

	}
	else if (isExploding) {
		
		if (explodingCounter > explodingTimeSeconds) {
			EntityManager::DeregisterEntity(m_entityName);
		}

		explodingCounter += deltaTime / 1000;
	}
	else {
		thisTransform->rotateBy(Vec3(0, (deltaTime / 1000) * rotationSpeed, 0));
	}
}


void BombController::OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) {
	IEntity* thisEntity = EntityManager::instance.GetEntityByName(entity1name);
	IEntity* otherEntity = EntityManager::instance.GetEntityByName(entity2name);

	BoxColliderComponent* thisCollider = thisEntity->GetComponent<BoxColliderComponent>();
	BoxColliderComponent* otherCollider = otherEntity->GetComponent<BoxColliderComponent>();
	if (isExploding) {
		if (otherCollider->layer == BoxColliderComponent::CollisionLayers::Destructable) {
			//destroy the other entity.
			EntityManager::DeregisterEntity(entity2name);
		}
	}
	else if (otherCollider->layer == BoxColliderComponent::CollisionLayers::Impassable || otherCollider->layer == BoxColliderComponent::CollisionLayers::Destructable) {
		//stick to the ground, start explode timer.
		if (isBeingThrown) {
			isBeingThrown = false;

			//modify the material to make the bomb red.
			MeshComponent* bombMesh = thisEntity->GetComponent<MeshComponent>();
			isFuseBurning = true;
		}
	}
}


void BombController::BeginThrow(Vec3 worldDirection) {
	isBeingThrown = true;
	throwDirection = worldDirection;
}