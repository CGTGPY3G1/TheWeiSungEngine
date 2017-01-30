#pragma once
#ifndef WS_GAMEOBJECT_FACTORY_H
#define WS_GAMEOBJECT_FACTORY_H

#include "GameObjectManager.h"
#include "TypeInfo.h"

class GameObjectFactory {
public:
	~GameObjectFactory() {}
	static std::weak_ptr<GameObject> CreateCharacter(const std::string & name, const int & characterType, const Vector2 & position, const Vector2 & scale = Vector2::One, const float & rotation = 0.0f);
	static std::weak_ptr<GameObject> CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation = 0.0f);
	static std::weak_ptr<GameObject> CreateBullet(const Vector2 & position, const Vector2 & scale, const float & rotation, const float & speed, const std::string & tag);
	static std::weak_ptr<GameObject> CreateVehicle(const int & vehicleNumber, const Vector2 & position, const Vector2 & scale, const float & rotation = 0.0f);
};

#endif // !WS_GAMEOBJECT_MANAGER_H