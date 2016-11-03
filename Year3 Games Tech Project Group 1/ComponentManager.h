#pragma once
#ifndef WS_COMPONENT_MANAGER_H
#define WS_COMPONENT_MANAGER_H

#include "TypeInfo.h"
struct TransformData {
	TransformData(const Vector2 & tPosition, const Vector2 & tScale, const float & tRotation) : position(tPosition), scale(tScale), rotation(tRotation) {}
	Vector2 position, scale;
	float rotation;
};

class GameObject;
class ComponentManager {
public:
	ComponentManager();
	ComponentManager(std::weak_ptr<GameObject> gameObject);
	~ComponentManager();
	template <typename T> std::weak_ptr<T> GetComponent();
	template <typename T> std::list<std::weak_ptr<T>> GetComponents();
	template <typename T> std::weak_ptr<T> AddComponent();
	template <typename T> T GetComponentData();
	template <> TransformData GetComponentData();
	void Start();
	void Update();
	void Update(double deltaTime);
	void FixedUpdate(double fixedDeltaTime);
	void LateUpdate();
private:
	int ownerID = 0;
	std::list<std::shared_ptr<Component>> components;
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
}

template<typename T>
T ComponentManager::GetComponentData() {
	return T();
}

template<>
TransformData ComponentManager::GetComponentData() {
	std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
	return TransformData(t->GetPosition(), t->GetScale(), t->GetRotation());
}

template<typename T>
std::weak_ptr<T> ComponentManager::GetComponent() {
	ComponentType type = TypeInfo::GetTypeID<T>();
	for(std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
		if(type == (*i)->Type()) {
			return std::static_pointer_cast<T>(*i);
		}
	}
	return std::weak_ptr<T>();
}

template<typename T>
std::list<std::weak_ptr<T>> ComponentManager::GetComponents() {
	std::list<std::weak_ptr<T>> toReturn = std::list<std::weak_ptr<T>>();
	if(TypeInfo::IsComponent<T>()) {
		ComponentType type = TypeInfo::GetTypeID<T>();
		for(std::list<std::shared_ptr<Component>>::iterator i = components.begin(); i != components.end(); ++i) {
			if(type == (*i)->Type()) toReturn.push_back(std::static_pointer_cast<T>(*i));
		}
	}
	return toReturn;
}
#endif // !WS_COMPONENT_MANAGER_H