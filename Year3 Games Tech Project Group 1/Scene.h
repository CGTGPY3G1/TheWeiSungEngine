#pragma once
#ifndef WS_SCENE_H
#define WS_SCENE_H
#include "Message.h"
#include <memory>
class GameObjectManager;
class PhysicsSystem;
class Scene {
public:
	Scene();
	~Scene();
	void Update(const float & step);
	void HandleMessage(const Message & message);
	std::weak_ptr<GameObjectManager> GetGameObjectManager();
	PhysicsSystem * GetPhysicsSystem();
private:
	PhysicsSystem * physicsSystem;
	std::shared_ptr<GameObjectManager> gameObjectManager;
};

#endif // !WS_SCENE_H