#include "stdafx.h"

#include "IEntity.h"

IEntity::IEntity(int initID, std::string& initName) : m_ID(initID), m_name(initName) {}

IEntity::~IEntity() {
	/*for (std::map<std::string, IComponent*>::iterator it = m_Components.begin(); it != m_Components.end(); it++) {
		delete it->second;
	}*/
}

void IEntity::OnBirth() {

}

void IEntity::OnDeath() {

}