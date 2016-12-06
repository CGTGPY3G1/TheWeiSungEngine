#pragma once
#ifndef WS_COMPONENT_H
#define WS_COMPONENT_H

#include <memory>
#include <vector>
#include <string>
#include "Message.h"
#include "Vector2.h"
#include <bitset>
#include <type_traits>

enum ComponentType : unsigned int {
	COMPONENT_NULL = 0,
	COMPONENT_TRANSFORM_2D = 1 << 0,
	COMPONENT_SPRITE_RENDERER = 1 << 1,
	COMPONENT_RIGID_BODY_2D = 1 << 2,
	COMPONENT_CIRCLE_COLLIDER = 1 << 3,
	COMPONENT_BOX_COLLIDER_2D = 1 << 4,
	COMPONENT_POLYGON_COLLIDER_2D = 1 << 5,
	COMPONENT_WHEEL_JOINT = 1 << 6,
	COMPONENT_REVOLUTE_JOINT = 1 << 7,
	COMPONENT_CHARACTER_MOVEMENT = 1 << 8,
	COMPONENT_CAMERA_FOLLOW = 1 << 9
};

#define NUMBER_OF_COMPONENTS = 6

class IDManager {
public:
	~IDManager() {}
	static int getNewCompID();
private:
	static int compCount;
};

struct ComponentData;
class GameObject;
class ScriptableComponent;
class Component : public std::enable_shared_from_this<Component> {
public:
	Component();
	Component(std::weak_ptr<GameObject> gameObject);
	virtual ~Component();
	virtual const ComponentType Type() const = 0;
	void SetEnabled(const bool &enabled);
	bool GetEnabled();
	void SetOwner(std::weak_ptr<GameObject> gameObject);
	unsigned int GetCompID();
	unsigned int GetGameObjectID();
	std::string GetName();
	void SetName(std::string name);
	std::weak_ptr<GameObject> GetGameObject();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> AddComponent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponentInParent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::vector<std::weak_ptr<T>> GetComponents();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> bool ComponentExistsInParents();
	virtual void Start();
	virtual void End();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void Destroy();
	virtual void HandleMessage(const Message & message) {};
	bool operator < (const Component & other) {
		return ownerID < other.ownerID && compID < other.compID;
	}
	bool operator > (const Component & other) {
		return ownerID > other.ownerID && compID > other.compID;
	}
	bool operator == (const Component & other) {
		return ownerID == other.ownerID && compID == other.compID;
	}

protected:
	std::weak_ptr<Component> GetWeak() { return shared_from_this(); }
	bool enabled = false;
	std::string name;
	unsigned int ownerID, compID;
	void Init(const bool & enabled, const std::string & name);
	std::weak_ptr<GameObject> gameObject;
	ComponentData * componentData;
};

template<typename T> std::weak_ptr<T> Component::AddComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->AddComponent<T>();
	return std::weak_ptr<T>();
}

template<typename T> std::weak_ptr<T> Component::GetComponent() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponent<T>();
	return std::weak_ptr<T>();
}

template<typename T>
std::weak_ptr<T> Component::GetComponentInParent() {
	return gameObject.lock()->GetComponentInParent<T>();
}

template<typename T> std::vector<std::weak_ptr<T>> Component::GetComponents() {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(g) return g->GetComponents<T>();
	return std::vector<std::weak_ptr<T>>();
}
template<typename T>
bool Component::ComponentExistsInParents() {
	return gameObject.lock()->ComponentExistsInParents<T>();
}
#endif // !WS_COMPONENT_H


