#include "Scene.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "TypeInfo.h"
#include "SpriteRenderingSystem.h"
#include "ScriptManagementSystem.h"
#include "AssetManager.h"
#include "Message.h"

Scene::Scene() {
	static unsigned int id = 0;
	sceneID = ++id;
}

Scene::~Scene() {
	End();
}

void Scene::Start() {
	scriptManagementSystem = std::make_shared<ScriptManagementSystem>();
	systems.push_back(std::make_shared<SpriteRenderingSystem>());
}

void Scene::Reset() {
	End();
	Start();
}

void Scene::FixedUpdate(const float & fixedDeltaTime) {
	PhysicsSystem::GetInstance().Update(fixedDeltaTime);
	if(scriptManagementSystem) scriptManagementSystem->FixedUpdate(fixedDeltaTime);
	SyncPhysics();
}

void Scene::SyncPhysics() {
	PhysicsSystem::GetInstance().UpdateBodies();
}

void Scene::Update(const float & deltaTime) {
	std::vector<std::shared_ptr<GameObject>> gameObjects = GameObjectManager::GetInstance().GetGameObjects();
	scriptManagementSystem->LoadScripts(gameObjects);
	scriptManagementSystem->Update(deltaTime);
	for(std::vector<std::shared_ptr<System>>::iterator i = systems.begin(); i != systems.end(); ++i) {
		if(((*i)->GetComponentMask() & ComponentType::COMPONENT_SPRITE_RENDERER) != ComponentType::COMPONENT_SPRITE_RENDERER)
			(*i)->ProcessComponents(gameObjects);
	}
}

void Scene::Render() {	
	std::vector<std::shared_ptr<GameObject>> gameObjects = GameObjectManager::GetInstance().GetGameObjects();
	for(std::vector<std::shared_ptr<System>>::iterator i = systems.begin(); i != systems.end(); ++i) {
		if(((*i)->GetComponentMask() & ComponentType::COMPONENT_SPRITE_RENDERER) == ComponentType::COMPONENT_SPRITE_RENDERER)
			(*i)->ProcessComponents(gameObjects);
	}
	scriptManagementSystem->Render();
	if(drawColliders) PhysicsSystem::GetInstance().Draw();
	scriptManagementSystem->LateUpdate();
}

void Scene::FinaliseFrame() {
	GameObjectManager::GetInstance().RemoveDeleted();
}

void Scene::End() {

}

void Scene::HandleMessage(const Message & message) {
	switch(message.type) {
	case MessageType::MESSAGE_TYPE_REGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_UNREGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_REGISTER_RIGIDBODY:
	case MessageType::MESSAGE_TYPE_UNREGISTER_RIGIDBODY:
		PhysicsSystem::GetInstance().HandleMessage(message);
		break;
	default:
		break;
	}
}

bool Scene::operator == (Scene other) {
	return sceneID == other.sceneID;
}
