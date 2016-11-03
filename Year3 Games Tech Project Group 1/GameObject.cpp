#include"GameObject.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "PhysicsSystem.h"
GameObject::GameObject(std::weak_ptr<GameObjectManager> gameObjectManager, const unsigned int & id, const std::string & goName) : manager(gameObjectManager), objectID(id), name(goName) {
	this->name = "New GameObject";
}

GameObject::~GameObject() {
}

std::string GameObject::GetName() {
	return name;
}

void GameObject::SetName(std::string name) {
	this->name = name;
}

bool GameObject::HasComponents(const unsigned int & mask) {
	if(componentMask == 0 || mask == 0) return false;
	return (componentMask & mask) == mask;
}

unsigned int GameObject::GetObjectID() {
	return objectID;
}

void GameObject::SetComponentMask(const unsigned int & newMask) {
	componentMask = newMask;
}

unsigned int GameObject::GetComponentMask() {
	return componentMask;
}

void GameObject::Init() {
	Init(Vector2(), 0);
}

void GameObject::Init(const Vector2 & position, const float & rotation) {
	componentManager = ComponentManager(GetWeak());
	std::shared_ptr<Transform2D> transform = componentManager.AddComponent<Transform2D>().lock();
	transform->SetPosition(position); transform->SetRotation(rotation);
}

void GameObject::HandleMessage(const Message & message) {
	switch(message.type) {
	case MessageType::MESSAGE_TYPE_REGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_UNREGISTER_COLLIDER:
	case MessageType::MESSAGE_TYPE_REGISTER_RIGIDBODY:
	case MessageType::MESSAGE_TYPE_UNREGISTER_RIGIDBODY:
		manager.lock()->GetScene()->GetPhysicsSystem()->HandleMessage(message);
		break;
	default:
		break;
	}
};


bool operator<(const GameObject & a, const GameObject & b) {
	return a.objectID < b.objectID;
}

bool operator == (const GameObject & a, const GameObject & b) {
	return a.objectID == b.objectID;
}
