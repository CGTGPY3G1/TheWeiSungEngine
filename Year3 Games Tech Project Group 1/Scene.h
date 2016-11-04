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
	void Start();
	void Reset();
	void Update(const float & deltaTime);
	void End();
	void HandleMessage(const Message & message);
	std::weak_ptr<GameObjectManager> GetGameObjectManager();
	PhysicsSystem * GetPhysicsSystem();
	unsigned int GetID() { return sceneID; }
	bool operator == (Scene other);
private:
	unsigned int sceneID;
	bool drawColliders = true;
	PhysicsSystem * physicsSystem;
	std::shared_ptr<GameObjectManager> gameObjectManager;
};

#endif // !WS_SCENE_H