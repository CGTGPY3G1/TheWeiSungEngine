#include "ComponentManager.h"
#include "ComponentHeaders.h"
#include "CollisionData.h"
#include "GameObject.h"
ComponentManager::ComponentManager() {
}

ComponentManager::ComponentManager(std::weak_ptr<GameObject> gameObject) {
	this->gameObject = gameObject;
}

ComponentManager::~ComponentManager() {

}

template<> std::weak_ptr<Transform2D> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<Transform2D>()) g->componentMask |= TypeInfo::GetTypeID<Transform2D>();
	std::shared_ptr<Transform2D> t = std::make_shared<Transform2D>(gameObject);
	components.push_back(t);
	return t;
}



template<> std::weak_ptr<RigidBody2D> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<RigidBody2D>()) g->componentMask |= TypeInfo::GetTypeID<RigidBody2D>();
	std::shared_ptr<RigidBody2D> t = std::make_shared<RigidBody2D>(gameObject);
	components.push_back(t);
	return t;
}

template<> std::weak_ptr<SpriteRenderer> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<SpriteRenderer>()) g->componentMask |= TypeInfo::GetTypeID<SpriteRenderer>();
	std::shared_ptr<SpriteRenderer> t = std::make_shared<SpriteRenderer>(gameObject);
	components.push_back(t);
	return t;
}

template<> std::weak_ptr<BoxCollider> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<BoxCollider>()) g->componentMask |= TypeInfo::GetTypeID<BoxCollider>();
	std::shared_ptr<BoxCollider> t = std::make_shared<BoxCollider>(gameObject);
	components.push_back(t);
	return t;
}

template<> std::weak_ptr<CircleCollider> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<CircleCollider>()) g->componentMask |= TypeInfo::GetTypeID<CircleCollider>();
	std::shared_ptr<CircleCollider> t = std::make_shared<CircleCollider>(gameObject);
	components.push_back(t);
	return t;
}

template<> std::weak_ptr<PolygonCollider> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<PolygonCollider>()) g->componentMask |= TypeInfo::GetTypeID<PolygonCollider>();
	std::shared_ptr<PolygonCollider> t = std::make_shared<PolygonCollider>(gameObject);
	components.push_back(t);
	return t;
}

void ComponentManager::Start() {
}

void ComponentManager::Update() {
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->update();
		//	}
	}
}

void ComponentManager::Update(double deltaTime) {
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->update(deltaTime);
		//	}
	}
}

void ComponentManager::FixedUpdate(double fixedDeltaTime) {
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->fixedUpdate(fixedDeltaTime);
		//	}
	}
}

void ComponentManager::LateUpdate() {
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->lateUpdate();
		//	}
	}
}

void ComponentManager::OnCollisionEnter(const CollisionData & data) {
}

void ComponentManager::OnCollisionExit(const CollisionData & data) {
}

void ComponentManager::OnSensorEnter(const std::weak_ptr<Collider> & collider) {
}

void ComponentManager::OnSensorExit(const std::weak_ptr<Collider> & collider) {
}
