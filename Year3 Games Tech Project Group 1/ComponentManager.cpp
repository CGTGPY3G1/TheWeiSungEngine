#include "ComponentManager.h"
#include "ComponentHeaders.h"

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
	for(std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->update();
		//	}
	}
}

void ComponentManager::Update(double deltaTime) {
	for(std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->update(deltaTime);
		//	}
	}
}

void ComponentManager::FixedUpdate(double fixedDeltaTime) {
	for(std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->fixedUpdate(fixedDeltaTime);
		//	}
	}
}

void ComponentManager::LateUpdate() {
	for(std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		//	if((*i)->getEnabled()) {
		//		(*i)->lateUpdate();
		//	}
	}
}
