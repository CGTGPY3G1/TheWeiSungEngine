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
