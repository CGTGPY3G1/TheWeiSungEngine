#pragma once
#ifndef WS_SCENE_H
#define WS_SCENE_H
#include "Message.h"
#include "AssetManager.h"
class GameObjectManager;
class PhysicsSystem;
class ScriptManagementSystem;
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
	virtual void FinaliseFrame();
	virtual void End();
	void HandleMessage(const Message & message);
	unsigned int GetID() { return sceneID; }
	bool operator == (Scene other);
protected:
	unsigned int sceneID;
	bool drawColliders = false;
	std::shared_ptr<ScriptManagementSystem> scriptManagementSystem;
	std::weak_ptr<Scene> GetWeak() { return shared_from_this(); }
	std::vector<std::shared_ptr<System>> systems;
};

#endif // !WS_SCENE_H