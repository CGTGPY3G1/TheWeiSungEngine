#pragma once
#ifndef WS_SCENE_H
#define WS_SCENE_H
#include "Message.h"
#include "AssetManager.h"
class GameObjectManager;
class PhysicsSystem;
class System;
class Scene : public std::enable_shared_from_this<Scene> {
public:
	Scene();
	virtual ~Scene();
	virtual void Start();
	virtual void Reset();
	virtual void FixedUpdate(const float & fixedDeltaTime);
	void SyncPhysics();
	virtual void Update(const float & deltaTime);
	virtual void Render();
	virtual void End();
	std::weak_ptr<AssetManager> GetAssetManager();
	void HandleMessage(const Message & message);
	std::weak_ptr<GameObjectManager> GetGameObjectManager();
	PhysicsSystem * GetPhysicsSystem();
	unsigned int GetID() { return sceneID; }
	bool operator == (Scene other);
protected:
	unsigned int sceneID;
	bool drawColliders = true;
	PhysicsSystem * physicsSystem;
	std::shared_ptr<GameObjectManager> gameObjectManager;
	std::shared_ptr<AssetManager> assetManager;
	std::weak_ptr<Scene> GetWeak() { return shared_from_this(); }
	std::vector<std::shared_ptr<System>> systems;
};

#endif // !WS_SCENE_H