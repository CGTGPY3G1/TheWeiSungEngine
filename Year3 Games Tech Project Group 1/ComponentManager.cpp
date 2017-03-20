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
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		(*i)->Destroy();
	}
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
		(*i)->Destroy();
	}
}

std::vector<std::shared_ptr<ScriptableComponent>> ComponentManager::GetScriptableComponents() {
	return scriptableComponents;
}

void ComponentManager::Start() {
}

void ComponentManager::Update() {

}

void ComponentManager::Update(double deltaTime) {

}

void ComponentManager::FixedUpdate(double fixedDeltaTime) {

}

void ComponentManager::LateUpdate() {

}

void ComponentManager::SetEnabled(const bool & enabled) {
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		(*i)->SetEnabled(enabled);
	}
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
		(*i)->SetEnabled(enabled);
	}
}

void ComponentManager::HandleMessage(const Message & message) {
	if(message.scope == MessageScope::MESSAGE_SCOPE_COMPONENTS)
		switch(message.type) {
		case MessageType::MESSAGE_TYPE_UPDATE_COLLISION_CATEGORY:
		{
			for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if(TypeInfo::IsCollider((*i)->Type())) {
					std::shared_ptr<Collider> collider = std::dynamic_pointer_cast<Collider>(*i);
					if(collider) {
						int data = *((int *)message.data);
						collider->SetCollisionCategory(data);		
					}
				}
			}
		}
			break;
		case MessageType::MESSAGE_TYPE_UPDATE_COLLISION_FILTER:
		{
			for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if(TypeInfo::IsCollider((*i)->Type())) {
					std::shared_ptr<Collider> collider = std::dynamic_pointer_cast<Collider>(*i);
					if(collider) {
						std::pair<int, int> data = *((std::pair<int, int>*)message.data);
						collider->SetCollisionFilter(data.first, data.second);
					}
				}
			}
		}
			break;
		case MessageType::MESSAGE_TYPE_UPDATE_COLLISION_MASK:
		{
			for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if(TypeInfo::IsCollider((*i)->Type())) {
					std::shared_ptr<Collider> collider = std::dynamic_pointer_cast<Collider>(*i);
					if(collider) {
						int data = *((int *)message.data);
						collider->SetCollisionMask(data);
					}
				}
			}
		}
			break;
		default:
			break;
	}
}

void ComponentManager::OnCollisionEnter(const CollisionData & data) {
	std::shared_ptr<Transform2D> parentTransform = std::static_pointer_cast<Transform2D>(components[0])->GetParent().lock();
	if(parentTransform) parentTransform->GetGameObject().lock()->OnCollisionEnter(data);
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) (*i)->OnCollisionEnter(data);
}

void ComponentManager::OnCollisionStay(const CollisionData & data) {
	std::shared_ptr<Transform2D> parentTransform = std::static_pointer_cast<Transform2D>(components[0])->GetParent().lock();
	if(parentTransform) parentTransform->GetGameObject().lock()->OnCollisionStay(data);
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) (*i)->OnCollisionStay(data);
}

void ComponentManager::OnCollisionExit(const CollisionData & data) {
	std::shared_ptr<Transform2D> parentTransform = std::static_pointer_cast<Transform2D>(components[0])->GetParent().lock();
	if(parentTransform) parentTransform->GetGameObject().lock()->OnCollisionExit(data);
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) (*i)->OnCollisionExit(data);
}

void ComponentManager::OnSensorEnter(const std::weak_ptr<Collider> & collider) {
	std::shared_ptr<Transform2D> parentTransform = std::static_pointer_cast<Transform2D>(components[0])->GetParent().lock();
	if(parentTransform) parentTransform->GetGameObject().lock()->OnSensorEnter(collider);
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) (*i)->OnSensorEnter(collider);
}

void ComponentManager::OnSensorExit(const std::weak_ptr<Collider> & collider) {
	std::shared_ptr<Transform2D> parentTransform = std::static_pointer_cast<Transform2D>(components[0])->GetParent().lock();
	if(parentTransform) parentTransform->GetGameObject().lock()->OnSensorExit(collider);
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) (*i)->OnSensorExit(collider);
}
