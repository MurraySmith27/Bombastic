#include "stdafx.h"
#include "EntityManager.h"


EntityManager::EntityManager() {
	EntityManager::all_entities = std::vector<IEntity>();
}


void EntityManager::RegisterEntity(IEntity entity) {
	EntityManager::all_entities.push_back(entity);
	std::vector<IScript*> scripts = entity.getScripts();

	for (auto i = scripts.begin(); i != scripts.end(); i++) {
		(*i)->Start();
	}
}

void EntityManager::DeregisterEntity(std::string name) {
	auto i = EntityManager::all_entities.begin();
	for (i; i != EntityManager::all_entities.end(); i++) {
		if ((*i).getName() == name) {
			EntityManager::all_entities.erase(i);
			break;
		}
	}
}

//template<class T>
//std::vector<T> EntityManager::GetAllComponents() {
//	std::vector<T> components();
//
//	auto i = all_entities.begin();
//	for (i; i != all_entities.end(); i++) {
//		T* component = (*i).GetComponent<T>();
//		if (component != NULL) {
//			components.push_back(&component);
//		}
//	}
//	return components;
//}

std::vector<IEntity> EntityManager::GetAllEntities() {
	return EntityManager::all_entities;
}


std::vector<IScript*> EntityManager::GetAllScripts() {
	std::vector<IScript*> scripts = std::vector<IScript*>();

	auto i = EntityManager::all_entities.begin();
	for (i; i != EntityManager::all_entities.end(); i++) {
		std::vector<IScript*> all_entity_scripts = (*i).getScripts();
		scripts.insert(scripts.end(), all_entity_scripts.begin(), all_entity_scripts.end());
	}

	return scripts;
}

IEntity* EntityManager::GetEntityByName(std::string name) {
	auto i = EntityManager::all_entities.begin();
	for (i; i != EntityManager::all_entities.end(); i++) {
		if ((*i).getName() == name) {
			return &(*i);
		}
	}
}

EntityManager EntityManager::instance;

std::vector<IEntity> EntityManager::all_entities;