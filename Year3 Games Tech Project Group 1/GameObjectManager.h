#pragma once
#ifndef WS_GAMEOBJECT_MANAGER_H
#define WS_GAMEOBJECT_MANAGER_H

#include <algorithm>
#include "GameObject.h"
#include "TypeInfo.h"

class Scene;
class GameObjectManager{
public:
	static GameObjectManager & GetInstance();
	~GameObjectManager();
	std::weak_ptr<GameObject> CreateGameObject(const std::string & name = "New GameObject");
	std::vector<std::shared_ptr<GameObject>> GetGameObjects();
	std::weak_ptr<GameObject> GetGameObject(const unsigned int & id, const bool retrieveDeleted = false);
	std::weak_ptr<GameObject> GetGameObject(const std::string & name, const bool retrieveDeleted = false);
	std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithName(const std::string & name);
	std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithComponent(const unsigned int & componentMask);
	void DeleteGameObject(const unsigned int & id);
protected:
	friend class Scene;
	GameObjectManager();
	
	std::vector<unsigned int> freeIDs = std::vector<unsigned int>();
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	void RemoveDeleted();
	std::vector<std::shared_ptr<GameObject>> toDelete;

};

struct SortAscending {
	bool operator()(const std::shared_ptr<GameObject> & l, const std::shared_ptr<GameObject> & r) { return l->GetObjectID() < r->GetObjectID(); }
	bool operator()(const std::shared_ptr<GameObject> & l, const unsigned int & r) { return l->GetObjectID() < r; }
};

struct SortDecending {
	bool operator()(const std::shared_ptr<GameObject> & l, const std::shared_ptr<GameObject> & r) { return l->GetObjectID() > r->GetObjectID(); }
	bool operator()(const std::shared_ptr<GameObject> & l, const unsigned int & r) { return l->GetObjectID() < r; }
};

struct IsEqual {
	bool operator()(const std::shared_ptr<GameObject> & l, const std::shared_ptr<GameObject> & r) { return l->GetObjectID() == r->GetObjectID(); }
	bool operator()(const std::shared_ptr<GameObject> & l, const unsigned int & r) { return l->GetObjectID() == r; }
};

struct HasGameObjectID {
	HasGameObjectID(const unsigned int & i) : ID(i) {}
	bool operator()(std::shared_ptr<GameObject> l) { return l->GetObjectID() == ID; }
private:
	unsigned int ID;
};

struct HasName {
	HasName(const std::string & goName) : name(goName) {}
	bool operator()(std::shared_ptr<GameObject> l) { return l->GetName().compare(name) == 0; }
private:
	std::string name;
};

#endif // !WS_GAMEOBJECT_MANAGER_H

