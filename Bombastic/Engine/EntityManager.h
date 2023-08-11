#ifndef ENTITYMANAGER_HEADER
#define ENTITYMANAGER_HEADER

#include "IEntity.h"
#include <string>
#include <vector>

class EntityManager
{


private:
	static std::vector<IEntity> all_entities;


public: 

	static EntityManager instance;
	EntityManager();

	static void RegisterEntity(IEntity entity);

	static void DeregisterEntity(std::string name);

	//template<class T>
	//std::vector<T> GetAllComponents() {
	//	std::vector<T> components = std::vector<T>();

	//	for (int i = 0; i < all_entities.size(); i++) {
	//		T* component = all_entities[i].GetComponent<T>();
	//		if (component != NULL) {
	//			components.push_back(*component);
	//		}
	//	}
	//	return components;
	//}

	static std::vector<IEntity> GetAllEntities();

	static std::vector<IScript*> GetAllScripts();

	static IEntity* GetEntityByName(std::string name);
};
#endif

