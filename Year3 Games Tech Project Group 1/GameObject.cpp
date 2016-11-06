#include"GameObject.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "CollisionData.h"
GameObject::GameObject(std::weak_ptr<GameObjectManager> gameObjectManager, const unsigned int & id, const std::string & goName) : manager(gameObjectManager), objectID(id), name(goName) {
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
		manager.lock()->GetScene().lock()->GetPhysicsSystem()->HandleMessage(message);
		break;
	default:
		break;
	}
}

void GameObject::OnCollisionEnter(const CollisionData & data) {
	std::cout << GetName() + " Started Colliding With " + data.gameObject.lock()->GetName() << std::endl << "Normal = " << data.normal << std::endl << "Relative Velocity = " << data.relativeVelocity << std::endl;
	componentManager.OnCollisionEnter(data);
}

void GameObject::OnCollisionExit(const CollisionData & data) {
	std::cout << GetName() + " Stopped Colliding With " + data.gameObject.lock()->GetName() << std::endl << "Normal = " << data.normal << std::endl << "Relative Velocity = " << data.relativeVelocity << std::endl;
	componentManager.OnCollisionExit(data);
}


bool operator<(const GameObject & a, const GameObject & b) {
	return a.objectID < b.objectID;
}

bool operator == (const GameObject & a, const GameObject & b) {
	return a.objectID == b.objectID;
}
