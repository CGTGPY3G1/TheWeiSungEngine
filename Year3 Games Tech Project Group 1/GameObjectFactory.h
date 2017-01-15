#pragma once
#ifndef WS_GAMEOBJECT_FACTORY_H
#define WS_GAMEOBJECT_FACTORY_H

#include "GameObjectManager.h"
#include "TypeInfo.h"

class GameObjectFactory {
public:
	~GameObjectFactory() {}
	static std::weak_ptr<GameObject> CreateCharacter(const std::string & name, const int & characterType, const Vector2 & position, const Vector2 & scale, const float & rotation);
	static std::weak_ptr<GameObject> CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation);
};

#endif // !WS_GAMEOBJECT_MANAGER_H