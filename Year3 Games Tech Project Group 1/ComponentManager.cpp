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

void ComponentManager::HandleMessage(const Message & message) {
	switch(message.scope) {
	case MessageScope::MESSAGE_SCOPE_COMPONENTS:
		switch(message.type) {
		case MessageType::MESSAGE_TYPE_UPDATE_COLLISION_CATEGORY:
		case MessageType::MESSAGE_TYPE_UPDATE_COLLISION_FILTER:
		case MessageType::MESSAGE_TYPE_UPDATE_COLLISION_MASK:
		{
			for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
				if(TypeInfo::IsCollider((*i)->Type())) {
					std::shared_ptr<Collider> collider = std::dynamic_pointer_cast<Collider>(*i);
					if(collider) {
						if(message.type == MessageType::MESSAGE_TYPE_UPDATE_COLLISION_CATEGORY) {
							int data = *((int *)message.data);
							collider->SetCollisionCategory(data);
						}
						else if(message.type == MessageType::MESSAGE_TYPE_UPDATE_COLLISION_FILTER) {
							std::pair<int, int> data = *((std::pair<int, int>*)message.data);
							collider->SetCollisionFilter(data.first, data.second);
						}
						else if(message.type == MessageType::MESSAGE_TYPE_UPDATE_COLLISION_MASK) {
							int data = *((int *)message.data);
							collider->SetCollisionMask(data);
						}
					}
				}
			}
		}	
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void ComponentManager::OnCollisionEnter(const CollisionData & data) {
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
		(*i)->OnCollisionEnter(data);
	}
}

void ComponentManager::OnCollisionExit(const CollisionData & data) {
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
		(*i)->OnCollisionExit(data);
	}
}

void ComponentManager::OnSensorEnter(const std::weak_ptr<Collider> & collider) {
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
		(*i)->OnSensorEnter(collider);
	}
}

void ComponentManager::OnSensorExit(const std::weak_ptr<Collider> & collider) {
	for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
		(*i)->OnSensorExit(collider);
	}
}
