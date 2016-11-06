#pragma once
#ifndef WS_COMPONENT_MANAGER_H
#define WS_COMPONENT_MANAGER_H
#include "TypeInfo.h"
#include "CollisionHandler.h"
class GameObject;
class ComponentManager : public CollisionHandler {
public:
	ComponentManager();
	ComponentManager(std::weak_ptr<GameObject> gameObject);
	~ComponentManager();
	template <typename T> std::weak_ptr<T> GetComponent();
	template <typename T> std::weak_ptr<T> GetComponentInParent();
	template <typename T> std::vector<std::weak_ptr<T>> GetComponents();
	template <typename T> std::weak_ptr<T> AddComponent();
	void Start();
	void Update();
	void Update(double deltaTime);
	void FixedUpdate(double fixedDeltaTime);
	void LateUpdate();
	void OnCollisionEnter(const CollisionData & data) override;
	void OnCollisionExit(const CollisionData & data) override;
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override;
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override;
private:
	int ownerID = 0;
	std::vector<std::shared_ptr<Component>> components;
	std::weak_ptr<GameObject> gameObject;
};


template<typename T> std::weak_ptr<T> ComponentManager::AddComponent() {
	if(TypeInfo::IsComponent<T>()) {
		std::shared_ptr<GameObject> g = gameObject.lock();
		if(!g->HasComponent<T>()) g->componentMask |= TypeInfo::GetTypeID<T>();
		std::shared_ptr<T> t = std::make_shared<T>(gameObject);
		components.push_back(t);
		return t;
	}
	return std::weak_ptr<T>();
}

template<typename T>
std::weak_ptr<T> ComponentManager::GetComponent() {
	ComponentType type = TypeInfo::GetTypeID<T>();
	for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		if(type == (*i)->Type()) {
			return std::static_pointer_cast<T>(*i);
		}
	}
	return std::weak_ptr<T>();
}

template<typename T>
inline std::weak_ptr<T> ComponentManager::GetComponentInParent() {
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
		parent = parent->GetParent();
	}
	return std::weak_ptr<T>();
}

template<typename T>
std::vector<std::weak_ptr<T>> ComponentManager::GetComponents() {
	std::vector<std::weak_ptr<T>> toReturn = std::vector<std::weak_ptr<T>>();
	if(TypeInfo::IsComponent<T>()) {
		ComponentType type = TypeInfo::GetTypeID<T>();
		for(std::vector<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
			if(type == (*i)->Type()) toReturn.push_back(std::static_pointer_cast<T>(*i));
		}
	}
	return toReturn;
}
#endif // !WS_COMPONENT_MANAGER_H