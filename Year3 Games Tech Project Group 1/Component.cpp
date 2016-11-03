#include "Component.h"
#include "GameObject.h"

int IDManager::compCount = 0;

int IDManager::getNewCompID() { return ++compCount; }

Component::Component() {
	Init(true, "Generic GameObject");
}

Component::Component(std::weak_ptr<GameObject> gameObject) {
	this->gameObject = gameObject;
	Init(enabled, "Generic GameObject");
}

Component::~Component() {
}

void Component::SetEnabled(const bool & enabled) {
	if(this->enabled != enabled) {
		this->enabled = enabled;
	}
}

bool Component::GetEnabled() {
	return enabled;
}

void Component::SetOwner(std::weak_ptr<GameObject> gameObject) {
	this->gameObject = gameObject;
}

unsigned int Component::GetCompID() {
	return compID;
}

unsigned int Component::GetGameObjectID() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	return g->GetObjectID();
}

std::string Component::GetName() {
	return name;
}

void Component::SetName(std::string name) {
	this->name = name;
}

std::weak_ptr<GameObject> Component::GetGameObject() {
	return gameObject;
}

void Component::Start() {
}

void Component::Update() {
}

void Component::FixedUpdate() {
}

void Component::LateUpdate() {
}

void Component::Init(const bool & enabled, const std::string & name) {
	this->name = name;
	SetEnabled(enabled);
	compID = IDManager::getNewCompID();
}

template<> std::weak_ptr<Transform2D> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->AddComponent<Transform2D>();
	return std::weak_ptr<Transform2D>();
}

template<> std::weak_ptr<RigidBody2D> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) 
		return g->AddComponent<RigidBody2D>();
	return std::weak_ptr<RigidBody2D>();
}

template<> std::weak_ptr<SpriteRenderer> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->AddComponent<SpriteRenderer>();
	return std::weak_ptr<SpriteRenderer>();
}

template<> std::weak_ptr<BoxCollider> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->AddComponent<BoxCollider>();
	return std::weak_ptr<BoxCollider>();
}

template<> std::weak_ptr<CircleCollider> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->AddComponent<CircleCollider>();
	return std::weak_ptr<CircleCollider>();
}

template<> std::weak_ptr<PolygonCollider> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->AddComponent<PolygonCollider>();
	return std::weak_ptr<PolygonCollider>();
}

template<> std::weak_ptr<Transform2D> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<Transform2D>();
	return std::weak_ptr<Transform2D>();
}

template<> std::weak_ptr<RigidBody2D> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<RigidBody2D>();
	return std::weak_ptr<RigidBody2D>();
}

template<> std::weak_ptr<SpriteRenderer> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<SpriteRenderer>();
	return std::weak_ptr<SpriteRenderer>();
}

template<> std::weak_ptr<BoxCollider> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<BoxCollider>();
	return std::weak_ptr<BoxCollider>();
}

template<> std::weak_ptr<CircleCollider> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<CircleCollider>();
	return std::weak_ptr<CircleCollider>();
}

template<> std::weak_ptr<PolygonCollider> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<PolygonCollider>();
	return std::weak_ptr<PolygonCollider>();
}

template<> std::list<std::weak_ptr<Transform2D>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<Transform2D>();
	return std::list<std::weak_ptr<Transform2D>>();
}

template<> std::list<std::weak_ptr<RigidBody2D>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<RigidBody2D>();
	return std::list<std::weak_ptr<RigidBody2D>>();
}

template<> std::list<std::weak_ptr<SpriteRenderer>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<SpriteRenderer>();
	return std::list<std::weak_ptr<SpriteRenderer>>();
}

template<> std::list<std::weak_ptr<BoxCollider>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<BoxCollider>();
	return std::list<std::weak_ptr<BoxCollider>>();
}

template<> std::list<std::weak_ptr<CircleCollider>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<CircleCollider>();
	return std::list<std::weak_ptr<CircleCollider>>();
}

template<> std::list<std::weak_ptr<PolygonCollider>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<PolygonCollider>();
	return std::list<std::weak_ptr<PolygonCollider>>();
}


