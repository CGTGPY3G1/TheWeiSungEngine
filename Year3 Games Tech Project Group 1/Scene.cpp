#include "Scene.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "TypeInfo.h"
Scene::Scene() {
	static unsigned int id = 0;
	sceneID = ++id;
	Start();
}

Scene::~Scene() {
	End();
}

void Scene::Start() {
	gameObjectManager = std::make_shared<GameObjectManager>(this);
	physicsSystem = new PhysicsSystem(this);
}

void Scene::Reset() {
	End();
	Start();
}

void Scene::Update(const float & deltaTime) {
	if(physicsSystem && drawColliders) physicsSystem->Draw();
}

void Scene::End() {
	if(physicsSystem) {
		delete physicsSystem; 
		physicsSystem = NULL;
	}
	
}

void Scene::HandleMessage(const Message & message) {
	switch(message.type) {
	case MessageType::MESSAGE_TYPE_REGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_UNREGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_REGISTER_RIGIDBODY:
	case MessageType::MESSAGE_TYPE_UNREGISTER_RIGIDBODY:
//		physicsSystem->HandleMessage(message);
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






