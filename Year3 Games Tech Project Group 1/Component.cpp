#include "Component.h"
#include "GameObject.h"
#include "ComponentData.h"
int IDManager::compCount = 0;

int IDManager::getNewCompID() { return ++compCount; }

Component::Component() {
	Init(true);
}

Component::Component(std::weak_ptr<GameObject> gameObject) {
	this->gameObject = gameObject;
	Init(enabled);
}

Component::~Component() {
}

void Component::SetEnabled(const bool & enabled) {
	if(this->enabled != enabled) {
		this->enabled = enabled;
		if(enabled) OnEnable();
		else OnDisable();
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

std::string Component::GetTag() {
	return gameObject.lock()->GetTag();
}

std::weak_ptr<GameObject> Component::GetGameObject() {
	return gameObject;
}

void Component::Start() {
}

void Component::End() {
}

void Component::OnEnable() {
}

void Component::OnDisable() {
}

void Component::Destroy() {

}

void Component::Init(const bool & enabled) {
	SetEnabled(enabled);
	compID = IDManager::getNewCompID();
}

