#include "stdafx.h"

#include "IScript.h"

IScript::IScript() {

}

IScript::IScript(std::string& entityName) {
	m_entityName = entityName;
}

IScript::~IScript() {

}