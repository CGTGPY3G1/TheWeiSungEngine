#pragma once
#ifndef WS_GAMEOBJECT_FACTORY_H
#define WS_GAMEOBJECT_FACTORY_H

#include "GameObjectManager.h"
#include "TypeInfo.h"
struct CollectableData {
	std::string name;
	CollectableType type;
	unsigned int amountOfType;
	Vector2 position, scale;
};
class GameObjectFactory {
public:
	~GameObjectFactory() {}
	static std::weak_ptr<GameObject> CreateCharacter(const std::string & name, const int & characterType, const bool & aiControlled, const Vector2 & position, const Vector2 & scale = Vector2::One, const float & rotation = 0.0f);
	static std::weak_ptr<GameObject> CreateBuilding(const int & buildingNumber, const Vector2 & position, const Vector2 & scale, const float & rotation = 0.0f);
	static std::weak_ptr<GameObject> CreateBarrier(const int & barrierType, const Vector2 & position, const Vector2 & scale, const float & rotation = 0.0f);
	static std::weak_ptr<GameObject> CreateBullet(const std::weak_ptr<GameObject> & creator, const std::string & creatorName, const Vector2 & position, const Vector2 & scale, const float & rotation, const float & speed, const std::string & tag);
	static std::weak_ptr<GameObject> CreateGrenade(const std::weak_ptr<GameObject> & creator, const std::string & creatorName, const Vector2 & position, const Vector2 & scale, const float & rotation, const float & speed, const std::string & tag);
	static std::weak_ptr<GameObject> CreateVehicle(const int & vehicleNumber, const Vector2 & position, const Vector2 & scale, const float & rotation = 0.0f);
	static std::weak_ptr<GameObject> CreateBloodSplat(const Vector2 & position, const int & type, const int & version = -1);
	static std::weak_ptr<GameObject> CreateBloodSpray(const Vector2 & position, const Vector2 & direction, const Vector2 & scale);
	static std::weak_ptr<GameObject> CreateExplosionAnim(const Vector2 & position, const float & rotation = 0.0f);
	static void ScheduleCollectableCreation(const std::string & name, const CollectableType & type, const unsigned int amountOfType, const Vector2 & position, const Vector2 & scale = Vector2::One);
	static void CreateScheduledCollectables();
private:
	static std::vector<CollectableData> data;
	static std::weak_ptr<GameObject> CreateCollectable(const std::string & name, const CollectableType & type, const unsigned int amountOfType, const Vector2 & position, const Vector2 & scale = Vector2::One);
};

#endif // !WS_GAMEOBJECT_MANAGER_H