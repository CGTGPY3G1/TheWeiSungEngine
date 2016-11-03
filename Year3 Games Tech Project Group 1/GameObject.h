#pragma once
#ifndef WS_GAMEOBJECT_H
#define WS_GAMEOBJECT_H
#include "ComponentManager.h"
#include <string>

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	friend class GameObjectManager;
	friend class ComponentManager;
	GameObject(std::weak_ptr<GameObjectManager> gameObjectManager, const unsigned int & objectID, const std::string & goName = "New GameObject");
	~GameObject();
	template<typename T> std::weak_ptr<T> AddComponent();
	template<typename T> std::weak_ptr<T> GetComponent();
	template<typename T> std::list<std::weak_ptr<T>> GetComponents();
	friend bool operator < (const GameObject & a, const GameObject & b);
	friend bool operator == (const GameObject & a, const GameObject & b);
	std::string GetName();
	void SetName(std::string name);
	template <typename T> bool HasComponent();
	bool HasComponents(const unsigned int & mask);
	unsigned int GetObjectID();
	unsigned int GetComponentMask();
	void SetComponentMask(const unsigned int & newMask);
	virtual void Init();
	virtual void Init(const Vector2 & position, const float & rotation);
	void HandleMessage(const Message & message);
	template <typename T> T GetComponentData();
protected:	
	std::weak_ptr<GameObject> GetWeak() { return shared_from_this(); }
	unsigned int objectID;
	unsigned int componentMask;
	ComponentManager componentManager;
	std::string name;
	std::weak_ptr<GameObjectManager> manager;
};

template<typename T>
std::weak_ptr<T> GameObject::AddComponent() {
	if((TypeInfo::AllowMultiple<T>() || !HasComponent<T>())) {
		return componentManager.AddComponent<T>();
	}
	return componentManager.GetComponent<T>();
}

template<typename T>
std::weak_ptr<T> GameObject::GetComponent() {
	return HasComponent<T>() ? componentManager.GetComponent<T>() : std::weak_ptr<T>();
}

template<typename T>
std::list<std::weak_ptr<T>> GameObject::GetComponents() {
	return HasComponent<T>() ? componentManager.GetComponents<T>() : std::list<std::weak_ptr<T>>();
}

template<typename T>
bool GameObject::HasComponent() {
	if(componentMask == 0) return false;
	unsigned int component = TypeInfo::GetTypeID<T>();
	return (componentMask & component) == component;
}

template<typename T>
inline T GameObject::GetComponentData() {
	return componentManager.GetComponentData<T>();
}

template<typename T>
inline void GameObject::AddToMask() {
}

#endif // !WS_GAMEOBJECT_H