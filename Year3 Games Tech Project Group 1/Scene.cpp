#include "Scene.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "TypeInfo.h"
Scene::Scene() {
	gameObjectManager = std::make_shared<GameObjectManager>(this);
	physicsSystem = new PhysicsSystem(this);
}

Scene::~Scene() {
}

void Scene::Update(const float & step) {
	
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
;





