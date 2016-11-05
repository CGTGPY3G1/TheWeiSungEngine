#pragma once
#ifndef WS_GAMEOBJECT_MANAGER_H
#define WS_GAMEOBJECT_MANAGER_H

#include <algorithm>
#include "GameObject.h"
#include "TypeInfo.h"

class Scene;
class GameObjectManager : public std::enable_shared_from_this<GameObjectManager> {
public:
	GameObjectManager(std::weak_ptr<Scene> owner);
	~GameObjectManager();
	std::weak_ptr<GameObject> CreateGameObject();
	std::weak_ptr<GameObject> CreateGameObject(const std::string name);
	std::weak_ptr<GameObject> GetGameObject(const unsigned int & id);
	std::weak_ptr<GameObject> GetGameObject(const std::string & name);
	std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithName(const std::string & name);
	std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithComponent(const unsigned int & componentMask);
	void HandleMessage(const Message & message) {
		switch(message.type) {
		case MessageType::MESSAGE_TYPE_REGISTER_COLLIDER:

			break;
		default:
			break;
		}
	};
	std::weak_ptr<Scene> GetScene();
private:
	std::weak_ptr<Scene> scene;
	unsigned int nextID = 0;
	std::vector<unsigned int> freeIDs;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	std::weak_ptr<GameObjectManager> GetWeak() { return shared_from_this(); }
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

