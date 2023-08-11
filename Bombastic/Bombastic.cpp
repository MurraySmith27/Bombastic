
#include "stdafx.h"
#include <windows.h> 
#include <math.h>  
#include "app\app.h"


#include <vector>

//engine imports
#include "Engine/Mesh.h"
#include "Engine/MeshComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/CameraComponent.h"
#include "Engine/Renderer.h"
#include "Engine/EntityManager.h"
#include "Engine/BoxColliderComponent.h"
#include "EntityData.h"
#include "Engine/IScript.h"
#include "GameState.h"

//script imports
#include "PlayerControlScript.h"
#include "BombSpawnerController.h"
#include "MapLoader.h"

EntityManager entityManager;

CSimpleSprite* testSprite;

Renderer renderer;


void HandleCollisions() {
	std::vector<IEntity> entities = entityManager.GetAllEntities();
	std::vector<BoxColliderComponent*> boxColliders(entities.size());
	std::vector<TransformComponent*> transforms(entities.size());

	for (int i = 0; i < entities.size(); i++) {
		boxColliders[i] = entities[i].GetComponent<BoxColliderComponent>();
		transforms[i] = entities[i].GetComponent<TransformComponent>();
	}

	for (int i = 0; i < boxColliders.size(); i++) {
		if (boxColliders[i] == NULL || transforms[i] == NULL) continue;

		//detect collisions between box colliders here
		for (int j = i + 1; j < boxColliders.size(); j++) {
			if (boxColliders[j] == NULL || transforms[j] == NULL) continue;

			Vec3 overlapCenterWorld, overlapDimensionsWorld;

			if (boxColliders[i]->isColliding(boxColliders[j], transforms[i], transforms[j], overlapCenterWorld, overlapDimensionsWorld)) {

				std::vector<IScript*> scripts1 = entities[i].getScripts();
				for (int k = 0; k < scripts1.size(); k++) {
					scripts1[k]->OnCollision(entities[i].getName(), entities[j].getName(), overlapCenterWorld, overlapDimensionsWorld);
				}

				std::vector<IScript*> scripts2 = entities[j].getScripts();
				for (int k = 0; k < scripts2.size(); k++) {
					scripts2[k]->OnCollision(entities[j].getName(), entities[i].getName(), overlapCenterWorld, overlapDimensionsWorld);
				}
			}
		}
	}
}

void ScriptUpdate(float deltaTime) {
	std::vector<IScript*> all_scripts = EntityManager::GetAllScripts();

	for (auto i = all_scripts.begin(); i != all_scripts.end(); i++) {
		(*i)->FrameUpdate(deltaTime);
	}
}

void DrawObjects() {
	std::vector<IEntity> entities = entityManager.GetAllEntities();
	std::vector<TransformComponent*> transforms(entities.size());
	std::vector<MeshComponent*> meshes(entities.size());
	std::vector<DirectionalLightComponent> lights;

	
	IEntity* camera = entityManager.GetEntityByName(EntityData::cameraEntityName);

	CameraComponent* camComponent = NULL;
	camComponent = camera->GetComponent<CameraComponent>();

	TransformComponent* camTransform = NULL;
	camTransform = camera->GetComponent<TransformComponent>();

	for (int i = 0; i < entities.size(); i++) {
		transforms[i] = entities[i].GetComponent<TransformComponent>();
		meshes[i] = entities[i].GetComponent<MeshComponent>();
		DirectionalLightComponent* light = NULL;
		light = entities[i].GetComponent<DirectionalLightComponent>();
		if (light != NULL) {
			lights.push_back(*light);
		}
	}


	for (int i = 0; i < entities.size(); i++) {
		if (transforms[i] == NULL || meshes[i] == NULL) continue;

		renderer.DrawMesh(*meshes[i], *transforms[i], *camComponent, *camTransform, lights);
	}
}

void CreatePlayerAndCameraEntities(Vec3 location, float tileSize) {

	IEntity player = IEntity(EntityData::EntityData::entityIdGenerator++, EntityData::playerName);
	

	TransformComponent* playerTransform = new TransformComponent(Vec3(location));
	
	player.AddComponent<TransformComponent>(playerTransform);

	BoxColliderComponent* playerCollider = new BoxColliderComponent(Vec3(0, 0, 0), Vec3(1, 1, 1));
	player.AddComponent<BoxColliderComponent>(playerCollider);

	PlayerControlScript* playerControlScript = new PlayerControlScript(player.getName());
	player.AddScript(playerControlScript);

	entityManager.RegisterEntity(player);


	//create camera

	IEntity camera = IEntity(EntityData::entityIdGenerator++, EntityData::cameraEntityName);

	TransformComponent* camTransform = new TransformComponent(Vec3(0, 0, 0));
	camTransform->parent = playerTransform; //set the camera to be parented by the player
	camera.AddComponent<TransformComponent>(camTransform);

	IEntity cameraFollowTarget = IEntity(EntityData::entityIdGenerator++, EntityData::cameraFollowTargetName);
	TransformComponent* cameraFollowTargetTransform = new TransformComponent(Vec3(0, 0, -1), camTransform);
	cameraFollowTarget.AddComponent(cameraFollowTargetTransform);
	entityManager.RegisterEntity(cameraFollowTarget);

	CameraComponent* camComponent = new CameraComponent(cameraFollowTargetTransform);
	camera.AddComponent<CameraComponent>(camComponent);

	entityManager.RegisterEntity(camera);
}


void CreateFloor(Vec3 location, float tileSize) {
	IEntity bottomWall = IEntity(EntityData::entityIdGenerator++, EntityData::bottomWallName + std::to_string(EntityData::entityIdGenerator - 1));
	TransformComponent* bottomWallTransform = new TransformComponent(location);

	bottomWallTransform->scaleBy(tileSize / 2, 1, tileSize / 2);
	bottomWall.AddComponent(bottomWallTransform);

	Material floorMat = Material(Vec3(0.7, 0.5, 0.5), 0.2, Vec3(0.7, 0.2, 0.0), Vec3(0.3, 0.3, 0.3), 100);
	Mesh floorMesh = Mesh(".\\Meshes\\plane.obj", floorMat);

	bottomWall.AddComponent(new MeshComponent(floorMesh));
	bottomWall.AddComponent(new BoxColliderComponent(Vec3(0, -1, 0), Vec3(1, 2, 1), BoxColliderComponent::CollisionLayers::Impassable));
	entityManager.RegisterEntity(bottomWall);
}

void CreateDestructable(Vec3 location, float tileSize) {
	IEntity wallCube = IEntity(EntityData::entityIdGenerator, EntityData::wallName + std::to_string(EntityData::entityIdGenerator - 1));

	TransformComponent* wallTransform = new TransformComponent(location);

	wallTransform->scaleBy(tileSize / 2, tileSize / 2, tileSize / 2);
	wallCube.AddComponent(wallTransform);

	Material floorMat = Material(Vec3(1, 0.5, 0.5), 0.2, Vec3(1, 0.2, 0.0), Vec3(0.3, 0.3, 0.3), 100);
	Mesh floorMesh = Mesh(".\\Meshes\\simple.obj", floorMat);

	wallCube.AddComponent(new MeshComponent(floorMesh));
	wallCube.AddComponent(new BoxColliderComponent(Vec3(0, 0, 0), Vec3(1, 1, 1), BoxColliderComponent::CollisionLayers::Destructable));
	entityManager.RegisterEntity(wallCube);
}

void CreateWall(Vec3 location, float tileSize) {
	IEntity wallCube = IEntity(EntityData::entityIdGenerator, EntityData::wallName + std::to_string(EntityData::entityIdGenerator - 1));

	TransformComponent* wallTransform = new TransformComponent(location);

	wallTransform->scaleBy(tileSize / 2, tileSize / 2, tileSize / 2);
	wallCube.AddComponent(wallTransform);

	Material floorMat = Material(Vec3(0.0, 0.5, 0.5), 0.2, Vec3(0.0, 0.2, 0.0), Vec3(0.3, 0.3, 0.3), 100);
	Mesh floorMesh = Mesh(".\\Meshes\\simple.obj", floorMat);

	wallCube.AddComponent(new MeshComponent(floorMesh));
	wallCube.AddComponent(new BoxColliderComponent(Vec3(0, 0, 0), Vec3(1, 1, 1), BoxColliderComponent::CollisionLayers::Impassable));
	entityManager.RegisterEntity(wallCube);
}

void CreateGoal(Vec3 location, float tileSize) {
	std::string entityName = EntityData::goalName;
	IEntity goal = IEntity(EntityData::entityIdGenerator++, entityName);

	Material goalMat(Vec3(0.1, 1, 0.1), 0.1, Vec3(0.6, 1, 0.6), Vec3(0, 0.1, 0.3), 100);
	Mesh goalMesh(".\\Meshes\\flag.obj", goalMat);
	TransformComponent* goalTransform = new TransformComponent(location);
	goalTransform->scaleBy(1, 2, 1);
	goal.AddComponent(goalTransform);
	goal.AddComponent(new MeshComponent(goalMesh));
	goal.AddComponent(new BoxColliderComponent(Vec3(0, 2, 0), Vec3(1, 1, 1), BoxColliderComponent::CollisionLayers::Goal));

	EntityManager::RegisterEntity(goal);
}


void CreateBombSpawner(Vec3 location, float tileSize) {

	std::string entityName = EntityData::bombSpawner + std::to_string(EntityData::entityIdGenerator);
	IEntity bombSpawner = IEntity(EntityData::entityIdGenerator++, entityName);

	Material bombSpawnerMat(Vec3(0.1, 0.1, 0.1), 0.1, Vec3(0.6, 0.6, 0.6), Vec3(0, 0.1, 0.3), 100);
	Mesh bombSpawnerMesh(".\\Meshes\\bombspawner.obj", bombSpawnerMat);
	TransformComponent* bombSpawnerTransform = new TransformComponent(location);
	bombSpawnerTransform->scaleBy(1, 2, 1);
	bombSpawner.AddComponent(bombSpawnerTransform);
	bombSpawner.AddComponent(new MeshComponent(bombSpawnerMesh));
	bombSpawner.AddComponent(new BoxColliderComponent(Vec3(0, 2, 0), Vec3(1, 1, 1), BoxColliderComponent::CollisionLayers::BombSpawner));

	bombSpawner.AddScript(new BombSpawnerController(entityName));

	EntityManager::RegisterEntity(bombSpawner);
}

void CreateSceneFromMap(std::vector<std::vector<char>> map, float tileSize) {

	for (int row = 0; row < map.size(); row++) {
		for (int col = 0; col < map[row].size(); col++) {
			Vec3 location = Vec3((row * tileSize), tileSize / 2, (col * tileSize));
			if (map[row][col] == '0') {

				CreateFloor(location, tileSize);
			}
			else if (map[row][col] == 'w') {
				location[1] = tileSize;
				CreateWall(location, tileSize);
			}
			else if (map[row][col] == 'd') {
				CreateFloor(location, tileSize);
				location[1] = tileSize;
				CreateDestructable(location, tileSize);
			}
			else if (map[row][col] == 's') {
				CreateFloor(location, tileSize);
				CreateBombSpawner(location, tileSize);
			}
			else if (map[row][col] == 'g') {
				CreateFloor(location, tileSize);
				CreateGoal(location, tileSize);
			}
			else if (map[row][col] == 'p') {
				CreateFloor(location, tileSize);
				location[1] = tileSize;
				CreatePlayerAndCameraEntities(location, tileSize);
			}
		}
	}
}


void StartLevel(int levelNum) {
	std::vector<std::vector<char>> mapData = MapLoader::LoadMap(".\\Maps\\map" + std::to_string(levelNum) + ".txt");
	CreateSceneFromMap(mapData, 10);

	//construct a light
	IEntity light = IEntity(EntityData::entityIdGenerator++, std::string("light1"));

	DirectionalLightComponent* dirLightComponent = new DirectionalLightComponent(Vec3(1, -1, -1), Vec3(1, 1, 1));
	light.AddComponent<DirectionalLightComponent>(dirLightComponent);

	entityManager.RegisterEntity(light);

}


void Init()
{
	StartLevel(GameState::currentLevel);
}


void Update(float deltaTime)
{
	HandleCollisions();
	ScriptUpdate(deltaTime);

	if (GameState::resetGame) {
		GameState::resetGame = false;
		GameState::currentLevel += 1;
		if (GameState::currentLevel > 3) {
			//game over, quit application
			exit(0);
		}
		else {
			//destroy all entities
			std::vector<IEntity> all_entities = EntityManager::GetAllEntities();
			for (auto i = all_entities.begin(); i != all_entities.end(); i++) {
				EntityManager::DeregisterEntity((*i).getName());
			}
			StartLevel(GameState::currentLevel);
		}
	}
}

void Render()
{
	DrawObjects();
}

void Shutdown()
{
	std::vector<IEntity> all_entities = EntityManager::GetAllEntities();
	for (auto i = all_entities.begin(); i != all_entities.end(); i++) {
		EntityManager::DeregisterEntity((*i).getName());
	}
}