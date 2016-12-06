#include "Scene.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "TypeInfo.h"
#include "SpriteRenderingSystem.h"
#include "ScriptManagementSystem.h"

Scene::Scene() {
	static unsigned int id = 0;
	sceneID = ++id;
}

Scene::~Scene() {
	End();
}

void Scene::Start() {
	gameObjectManager = std::make_shared<GameObjectManager>(GetWeak());
	assetManager = std::make_shared<AssetManager>();
	scriptManagementSystem = std::make_shared<ScriptManagementSystem>();
	physicsSystem = new PhysicsSystem();
	systems.push_back(std::make_shared<SpriteRenderingSystem>());
}

void Scene::Reset() {
	End();
	Start();
}

void Scene::FixedUpdate(const float & fixedDeltaTime) {
	physicsSystem->Update(fixedDeltaTime);
	scriptManagementSystem->FixedUpdate(fixedDeltaTime);
}

void Scene::SyncPhysics() {
	physicsSystem->UpdateBodies();
}

void Scene::Update(const float & deltaTime) {
	std::vector<std::shared_ptr<GameObject>> gameObjects = gameObjectManager->GetGameObjects();
	scriptManagementSystem->LoadScripts(gameObjects);
	scriptManagementSystem->Update(deltaTime);
	for(std::vector<std::shared_ptr<System>>::iterator i = systems.begin(); i != systems.end(); ++i) {
		(*i)->ProcessComponents(gameObjects);
	}
}

void Scene::Render() {
	if(physicsSystem && drawColliders) physicsSystem->Draw();
	scriptManagementSystem->LateUpdate();
}

void Scene::End() {
	if(physicsSystem) {
		delete physicsSystem; 
		physicsSystem = NULL;
	}
	
}

std::weak_ptr<AssetManager> Scene::GetAssetManager() {
	return assetManager;
}

void Scene::HandleMessage(const Message & message) {
	switch(message.type) {
	case MessageType::MESSAGE_TYPE_REGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_UNREGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_REGISTER_RIGIDBODY:
	case MessageType::MESSAGE_TYPE_UNREGISTER_RIGIDBODY:
		physicsSystem->HandleMessage(message);
		break;
	default:
		break;
	}
}

std::weak_ptr<GameObjectManager> Scene::GetGameObjectManager() {
	return gameObjectManager;
}
PhysicsSystem * Scene::GetPhysicsSystem() {
	return physicsSystem;
}

bool Scene::operator == (Scene other) {
	return sceneID == other.sceneID;
}






