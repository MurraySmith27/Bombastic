#include "stdafx.h"
#include "App/app.h"
#include "PlayerControlScript.h"
#include <iostream>
#include "Engine/BoxColliderComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/EntityManager.h"
#include "EntityData.h";
#include "Engine/CollisionHandler.h"
#include "Engine/MeshComponent.h"
#include "GameState.h"

#include "BombSpawnerController.h"
#include "BombController.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>


PlayerControlScript::PlayerControlScript(std::string entityName) : IScript(entityName) {

}

void PlayerControlScript::Start() {

}

void PlayerControlScript::HandleMoveAndLook(float deltaTime) {

	float lookHorizontal = App::GetController().GetRightThumbStickX();
	lookHorizontal = lookHorizontal * (abs(lookHorizontal) > stickInputThreshhold);

	float lookVertical = App::GetController().GetRightThumbStickY();
	lookVertical = lookVertical * (abs(lookVertical) > stickInputThreshhold);

	TransformComponent* cameraFollowTargetTransform = EntityManager::GetEntityByName(EntityData::cameraFollowTargetName)->GetComponent<TransformComponent>();
	Vec4 targetPosLocal = cameraFollowTargetTransform->transform * Vec4(0, 0, 0, 1);


	//spherical coords: (theta, phi, r)
	float zOverX = targetPosLocal[2] / targetPosLocal[0];
	float theta = atan(zOverX);
	if (targetPosLocal[0] < 0) {
		theta += PI;
	}

	float phi = acos(targetPosLocal[1]);

	Vec3 lookVec = Vec3(-lookVertical * lookVerticalSpeed, lookHorizontal * lookHorizontalSpeed, 0);

	theta += lookHorizontal * lookHorizontalSpeed * (deltaTime / 1000) / (2 * M_PI);
	phi += -lookVertical * lookVerticalSpeed * (deltaTime / 1000) / M_PI;

	phi = max(min(phi, PI - 0.01f), 0.01f);

	Vec3 newTargetPos = Vec3(
		sin(phi) * cos(theta),
		cos(phi),
		sin(phi) * sin(theta)
	);

	if (lookVec.norm() != 0) {
		cameraFollowTargetTransform->moveTo(Vec3(newTargetPos));
	}

	if (this->playerHasBomb) {
		float phi_offset = -0.5f;
		float theta_offset = -0.5f;
		//TODO: polish here if there's time left.
		Vec3 newBombPos = Vec3(
			sin(phi - phi_offset) * cos(theta - theta_offset),
			cos(phi - phi_offset),
			sin(phi - phi_offset) * sin(theta - theta_offset)
		);
		this->bombTransform->moveTo(Vec3(newBombPos).normalize() * 5);
	}

	float moveX = App::GetController().GetLeftThumbStickX();
	moveX = moveX * (abs(moveX) > stickInputThreshhold);

	float moveY = App::GetController().GetLeftThumbStickY();
	moveY = moveY * (abs(moveY) > stickInputThreshhold);

	Vec3 rightDir = Vec3(newTargetPos).cross(Vec3(0, 1, 0));
	Vec3 moveVec = Vec3(newTargetPos) * moveY + rightDir * moveX;
	moveVec[1] = 0;

	moveVec = moveVec.normalize() * moveSpeed * (deltaTime / 1000);
	EntityManager::GetEntityByName(EntityData::playerName)->GetComponent<TransformComponent>()->moveBy(moveVec);
}

void PlayerControlScript::CheckForShootEvent() {
	if (this->playerHasBomb && App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		//throw the bomb.
		BombController* bombScript = EntityManager::GetEntityByName(this->bombName)->getScriptOfType<BombController>();
		TransformComponent* followTargetTransform = EntityManager::GetEntityByName(EntityData::cameraFollowTargetName)->GetComponent<TransformComponent>();
		bombScript->BeginThrow(followTargetTransform->transform * Vec4(0,0,0,1));
		TransformComponent* bombTransform = EntityManager::GetEntityByName(this->bombName)->GetComponent<TransformComponent>();
		TransformComponent* playerTransform = EntityManager::GetEntityByName(EntityData::playerName)->GetComponent<TransformComponent>();
		bombTransform->moveTo(Vec3(playerTransform->getWorldSpaceCoords(Vec4(0,0,0,1))));
		bombTransform->parent = NULL;
		this->playerHasBomb = false;
	}
}

void PlayerControlScript::FrameUpdate(float deltaTime) {
	HandleMoveAndLook(deltaTime);

	if (this->playerHasBomb) {
		CheckForShootEvent();
	}

	if (levelComplete && App::GetController().CheckButton(XINPUT_GAMEPAD_A, true)) {
		//reset level
		GameState::resetGame = true;
	}
}


void PlayerControlScript::OnCollision(std::string& entity1name, std::string& entity2name, Vec3 overlapCenterWorld, Vec3 overlapDimensionsWorld) {
	IEntity* thisEntity = EntityManager::instance.GetEntityByName(entity1name);
	IEntity* otherEntity = EntityManager::instance.GetEntityByName(entity2name);

	BoxColliderComponent* thisCollider = thisEntity->GetComponent<BoxColliderComponent>();
	BoxColliderComponent* otherCollider = otherEntity->GetComponent<BoxColliderComponent>();

	TransformComponent* thisTransform = thisEntity->GetComponent<TransformComponent>();
	TransformComponent* otherTransform = otherEntity->GetComponent<TransformComponent>();
	//in this case, the player should move away from whatever it collided with, if it was an impassable.
	if (otherCollider->layer == BoxColliderComponent::CollisionLayers::Impassable || otherCollider->layer == BoxColliderComponent::CollisionLayers::Destructable) {
		CollisionHandler::HandleCollisionMoveOut(thisTransform, thisCollider, otherTransform, otherCollider, overlapCenterWorld, overlapDimensionsWorld);
	}
	else if (otherCollider->layer == BoxColliderComponent::CollisionLayers::BombSpawner) {
		BombSpawnerController* spawnerScript = NULL;
		spawnerScript = otherEntity->getScriptOfType<BombSpawnerController>();
		if (spawnerScript->hasBomb && !this->playerHasBomb) {
			//pick up the bomb
			this->bombName = spawnerScript->getBombName();
			spawnerScript->hasBomb = false;
			bombTransform = EntityManager::GetEntityByName(bombName)->GetComponent<TransformComponent>();
			bombTransform->parent = thisTransform;
			this->playerHasBomb = true;
		}
	}
	else if (otherCollider->layer == BoxColliderComponent::CollisionLayers::Goal) {
		//player has collided with the goal! Display the text on screen.
		Material goalTextMaterial(Vec3(0, 1, 0), 0.1, Vec3(0, 1, 0), Vec3(1, 1, 1), 10000);

		std::string textPath = ".\\Meshes\\levelCompleteText.obj";
		if (GameState::currentLevel == GameState::maxLevels){
			textPath = ".\\Meshes\\gameCompleteText.obj";
		}
		Mesh goalTextMesh(textPath, goalTextMaterial);
		MeshComponent* goalTextMeshComponent = new MeshComponent(goalTextMesh);

		TransformComponent* goalTextTransform = new TransformComponent(Vec3(5, 2, 1));

		goalTextTransform->rotateBy(90, 0, 0);
		goalTextTransform->scaleBy(-1, 1, 1);

		goalTextTransform->parent = EntityManager::GetEntityByName(EntityData::goalName)->GetComponent<TransformComponent>();
		IEntity goalText = IEntity(EntityData::entityIdGenerator++, EntityData::goalTextName);

		goalText.AddComponent(goalTextMeshComponent);
		goalText.AddComponent(goalTextTransform);

		EntityManager::RegisterEntity(goalText);

		levelComplete = true;
	}
}