#include"GameObject.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "CollisionData.h"
GameObject::GameObject(const unsigned int & id, const std::string & goName) : objectID(id), name(goName) {
}

GameObject::~GameObject() {
}

std::vector<std::shared_ptr<ScriptableComponent>> GameObject::GetScriptableComponents() {
	return componentManager.GetScriptableComponents();
}

std::string GameObject::GetName() {
	return name;
}

void GameObject::SetName(std::string name) {
	this->name = name;
}

std::string GameObject::GetTag() {
	return tag;
}

void GameObject::SetTag(std::string tag) {
	this->tag = tag;
}

bool GameObject::GetEnabled() {
	return enabled;
}

void GameObject::SetEnabled(const bool & enabled) {
	this->enabled = enabled;
	componentManager.SetEnabled(enabled);
}

bool GameObject::HasComponents(const unsigned int & mask) {
	if(componentMask == 0 || mask == 0) return false;
	return (componentMask & mask) == mask;
}

unsigned int GameObject::GetObjectID() {
	return objectID;
}

void GameObject::SetObjectID(const unsigned int & newID) {
	objectID = newID;
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

void GameObject::Init(const Vector2 & position, const float & rotation, const Vector2 & scale) {
	componentManager = ComponentManager(GetWeak());
	std::shared_ptr<Transform2D> transform = componentManager.AddComponent<Transform2D>().lock();
	transform->SetPosition(position); transform->SetRotation(rotation); transform->SetScale(scale);
}

void GameObject::HandleMessage(const Message & message) {
	switch(message.scope) {
	case MessageScope::MESSAGE_SCOPE_PHYSICS_SYSTEM:
		PhysicsSystem::GetInstance().HandleMessage(message);
		break;
	default:
		break;
	}
}

void GameObject::OnCollisionEnter(const CollisionData & data) {
	//std::cout << GetName() + " Started Colliding With " + data.gameObject.lock()->GetName() << std::endl << "Normal = " << data.normal << std::endl << "Relative Velocity = " << data.relativeVelocity << std::endl;
	componentManager.OnCollisionEnter(data);
}

void GameObject::OnCollisionExit(const CollisionData & data) {
	//std::cout << GetName() + " Stopped Colliding With " + data.gameObject.lock()->GetName() << std::endl << "Normal = " << data.normal << std::endl << "Relative Velocity = " << data.relativeVelocity << std::endl;
	componentManager.OnCollisionExit(data);
}

void GameObject::OnSensorEnter(const std::weak_ptr<Collider> & collider) {
	//std::cout << GetName() + " Started Colliding With " + collider.lock()->GetGameObject().lock()->GetName() + (collider.lock()->IsSensor() ? " Sensor" : "") << std::endl;
	componentManager.OnSensorEnter(collider);
}

void GameObject::OnSensorExit(const std::weak_ptr<Collider> & collider) {
	//std::cout << GetName() + " Stopped Colliding With " + collider.lock()->GetGameObject().lock()->GetName() + (collider.lock()->IsSensor() ? " Sensor" : "") << std::endl;
	componentManager.OnSensorExit(collider);
}

void GameObject::SetCollisionFilter(const int & collisionCategory, const int & collisionMask) {
	this->collisionCategory = collisionCategory; this->collisionMask = collisionMask;
	std::pair<int, int> *data = new std::pair<int, int>(collisionCategory, collisionMask);
	Message m = Message(MessageScope::MESSAGE_SCOPE_COMPONENTS, MessageType::MESSAGE_TYPE_UPDATE_COLLISION_FILTER, data);
	componentManager.HandleMessage(m);
	delete data;
}

void GameObject::SetCollisionCategory(const int & collisionCategory) {
	this->collisionCategory = collisionCategory;
	int * data = new int(collisionCategory);
	Message m = Message(MessageScope::MESSAGE_SCOPE_COMPONENTS, MessageType::MESSAGE_TYPE_UPDATE_COLLISION_CATEGORY, data);
	componentManager.HandleMessage(m);
	delete data;
}

int GameObject::GetCollisionCategory() {
	return collisionCategory;
}

void GameObject::SetCollisionMask(const int & collisionMask) {
	this->collisionMask = collisionMask;
	int * data = new int(collisionMask);
	Message m = Message(MessageScope::MESSAGE_SCOPE_COMPONENTS, MessageType::MESSAGE_TYPE_UPDATE_COLLISION_MASK, data);
	componentManager.HandleMessage(m);
	delete data;
}

int GameObject::GetCollisionMask() {
	return collisionMask;
}

void GameObject::AllowCollisionsWith(const int & category, const bool & allowCollisions) {
	collisionMask = allowCollisions ? (collisionMask | category) : ~(collisionMask | category);
	int * data = new int(collisionMask);
	Message m = Message(MessageScope::MESSAGE_SCOPE_COMPONENTS, MessageType::MESSAGE_TYPE_UPDATE_COLLISION_MASK, data);
	componentManager.HandleMessage(m);
	delete data;
}

const bool GameObject::CollidesWith(const int & collisionMask) {
	return (this->collisionMask & collisionMask) == collisionMask;
}

void GameObject::Destroy() {
	std::shared_ptr<Transform2D> t = componentManager.GetComponent<Transform2D>().lock();
	if(t) {
		std::vector<std::weak_ptr<GameObject>> children;
		unsigned int childCount = t->GetChildCount();
		for(size_t i = 0; i < childCount; i++) {
			std::shared_ptr<Transform2D> child = t->GetChild(i).lock();
			if(child) children.push_back(child->GetGameObject());
		}
		for(std::vector<std::weak_ptr<GameObject>>::iterator it = children.begin(); it != children.end(); ++it) {
			std::shared_ptr<GameObject> g = (*it).lock();
			if(g) g->Destroy();
		}
	}
	GameObjectManager::GetInstance().DeleteGameObject(objectID);
}

GameObject::GameObject(const std::string & goName) : name(goName) {
}

bool operator<(const GameObject & a, const GameObject & b) {
	return a.objectID < b.objectID;
}

bool operator == (const GameObject & a, const GameObject & b) {
	return a.objectID == b.objectID;
}

