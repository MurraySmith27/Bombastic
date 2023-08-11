#include "stdafx.h"
#include "App/app.h"
#include "BombSpawnerController.h"
#include <iostream>
#include "Engine/BoxColliderComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/EntityManager.h"
#include "EntityData.h";
#include "Engine/CollisionHandler.h"
#include <string>
#include "Engine/Material.h"
#include "Engine/MeshComponent.h"
#include "Engine/Mesh.h"

#include "BombController.h"

BombSpawnerController::BombSpawnerController() : IScript() {

}

BombSpawnerController::BombSpawnerController(std::string entityName) : IScript(entityName) {

}

void BombSpawnerController::SpawnBomb() {
	this->bombName = EntityData::bombName + std::to_string(EntityData::entityIdGenerator - 1);
	IEntity bomb = IEntity(EntityData::entityIdGenerator++, bombName);
	bomb.AddComponent(new TransformComponent(Vec3(0, 2, 0), EntityManager::GetEntityByName(this->m_entityName)->GetComponent<TransformComponent>()));
	
	Material bombMat(Vec3(0, 0, 0.3), 0.6, Vec3(0, 0, 0.5), Vec3(1, 1, 1), 10000);
	Mesh bombMesh(".\\Meshes\\bomb_lowpoly.obj", bombMat);

	bomb.AddComponent(new MeshComponent(bombMesh));

	bomb.AddComponent(new BoxColliderComponent(Vec3(0,0,0), Vec3(0.1, 0.1, 0.1)));

	bomb.AddScript(new BombController(bombName));

	EntityManager::RegisterEntity(bomb);
}

void BombSpawnerController::Start() {
	SpawnBomb();
	hasBomb = true;
	counter = 0;
}

void BombSpawnerController::FrameUpdate(float deltaTime) {

	if (!hasBomb && counter > bombSpawnCooldownSeconds) {
		counter = 0;
		hasBomb = true;
		SpawnBomb();
	}
	else {
		counter += deltaTime / 1000;
	}
}


void BombSpawnerController::OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) {

}

std::string BombSpawnerController::getBombName() {
	return this->bombName;
}