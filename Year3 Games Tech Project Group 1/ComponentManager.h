#pragma once
#ifndef WS_COMPONENT_MANAGER_H
#define WS_COMPONENT_MANAGER_H
#include "TypeInfo.h"
#include "CollisionHandler.h"
#include "cereal\cereal.hpp"
#include "cereal\access.hpp"
#include "cereal\details\traits.hpp"
#include <cereal\types\polymorphic.hpp>
#include <cereal\types\vector.hpp>
//class GameObject;
class ComponentManager : public CollisionHandler {
public:
	ComponentManager();
	ComponentManager(std::weak_ptr<GameObject> gameObject);
	~ComponentManager();
	friend class cereal::access;
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponentInParent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::vector<std::weak_ptr<T>> GetComponents();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> AddComponent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> bool ComponentExistsInParents();
	std::vector<std::shared_ptr<ScriptableComponent>> GetScriptableComponents();
	void Start();
	void Update();
	void Update(double deltaTime);
	void FixedUpdate(double fixedDeltaTime);
	void LateUpdate();
	void HandleMessage(const Message & message);
	void OnCollisionEnter(const CollisionData & data) override;
	void OnCollisionExit(const CollisionData & data) override;
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override;
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override;
private:
	int ownerID = 0;
	std::vector<std::shared_ptr<Component>> components;
	std::vector<std::shared_ptr<ScriptableComponent>> scriptableComponents;
	std::weak_ptr<GameObject> gameObject;

	template <class Archive>
	void load(Archive & ar) {
		ar(cereal::make_nvp("OwnerID", ownerID),
		   cereal::make_nvp("Components", components),
		   cereal::make_nvp("ScriptableComponents", scriptableComponents));
	}

	template <class Archive>
	void save(Archive & ar) const {
		ar(cereal::make_nvp("OwnerID", ownerID),
		   cereal::make_nvp("Components", components),
		   cereal::make_nvp("ScriptableComponents", scriptableComponents));
	}
};

template<typename T> std::weak_ptr<T> ComponentManager::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<T>()) g->componentMask |= TypeInfo::GetTypeID<T>();
	std::shared_ptr<T> t = std::make_shared<T>(gameObject);
	if(TypeInfo::IsScriptable<T>()) {
		std::shared_ptr<ScriptableComponent> sc = std::dynamic_pointer_cast<ScriptableComponent>(t);
		scriptableComponents.push_back(sc);
	}
	else components.push_back(t);
	return t;
}

template<typename T>
std::weak_ptr<T> ComponentManager::GetComponent() {
	ComponentType type = TypeInfo::GetTypeID<T>();
	if(TypeInfo::IsScriptable<T>()) {
		for(std::vector<std::shared_ptr<ScriptableComponent>>::iterator i = scriptableComponents.begin(); i != scriptableComponents.end(); ++i) {
			if(type == (*i)->Type()) {
				std::weak_ptr<T> toReturn = std::dynamic_pointer_cast<T>(*i);
				return toReturn.expired() ? std::weak_ptr<T>() : toReturn;
			}
		}
	}
	else {
		for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
			if(type == (*i)->Type()) {
				return std::static_pointer_cast<T>(*i);
			}
		}
	}
	return std::weak_ptr<T>();
}

template<typename T>
std::weak_ptr<T> ComponentManager::GetComponentInParent() {
	std::shared_ptr<Transform2D> parent;
	ComponentType transformType = TypeInfo::GetTypeID<Transform2D>();
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		if(transformType == (*i)->Type()) {
			parent = std::static_pointer_cast<Transform2D>(*i)->GetParent().lock();
			break;
		}
	}
	while(parent) {
		std::weak_ptr<T> comp = parent->GetComponent<T>();
		if(!comp.expired()) return comp;
		parent = parent->GetParent().lock();
	}
	return std::weak_ptr<T>();
}

template<typename T>
std::vector<std::weak_ptr<T>> ComponentManager::GetComponents() {
	std::vector<std::weak_ptr<T>> toReturn = std::vector<std::weak_ptr<T>>();
	ComponentType type = TypeInfo::GetTypeID<T>();
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		if(type == (*i)->Type()) toReturn.push_back(std::static_pointer_cast<T>(*i));
	}

	return toReturn;
}

template<typename T>
bool ComponentManager::ComponentExistsInParents() {	
	std::shared_ptr<Transform2D> parent;
	ComponentType transformType = TypeInfo::GetTypeID<Transform2D>();
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		if(transformType == (*i)->Type()) {
			parent = std::static_pointer_cast<Transform2D>(*i)->GetParent().lock();
			break;
		}
	}
	bool found = false;
	while(parent && !found) {
		found = parent->GetGameObject().lock()->HasComponent<T>();
		parent = parent->GetParent().lock();
	}
	return found;
}
#endif // !WS_COMPONENT_MANAGER_H