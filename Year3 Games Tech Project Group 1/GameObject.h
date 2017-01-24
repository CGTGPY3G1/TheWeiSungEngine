#pragma once
#ifndef WS_GAMEOBJECT_H
#define WS_GAMEOBJECT_H
#include "ComponentManager.h"
#include <string>

enum CollisionCategory {
	CATEGORY_ALL = 1 << 1,
	CATEGORY_WHEEL = 1 << 2,
	CATEGORY_CAR = 1 << 3,
	CATEGORY_AI_CHARACTER = 1 << 4,
	CATEGORY_PLAYER = 1 << 5,
};


class GameObject : public std::enable_shared_from_this<GameObject>, public CollisionHandler {
public:
	friend class GameObjectManager;
	friend class ComponentManager;
	friend class PhysicsSystem;
	friend class cereal::access;
	GameObject(const std::string & goName = "New GameObject");
	GameObject(const unsigned int & objectID, const std::string & goName = "New GameObject");
	~GameObject();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> AddComponent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::weak_ptr<T> GetComponentInParent();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> std::vector<std::weak_ptr<T>> GetComponents();
	std::vector<std::shared_ptr<ScriptableComponent>> GetScriptableComponents();
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> bool ComponentExistsInParents();
	friend bool operator < (const GameObject & a, const GameObject & b);
	friend bool operator == (const GameObject & a, const GameObject & b);
	std::string GetName();
	void SetName(std::string name);
	std::string GetTag();
	void SetTag(std::string tag);
	bool GetEnabled();
	void SetEnabled(const bool & enabled);
	template <typename T> bool HasComponent();
	bool HasComponents(const unsigned int & mask);
	unsigned int GetObjectID();
	void SetObjectID(const unsigned int & newID);
	unsigned int GetComponentMask();
	void SetComponentMask(const unsigned int & newMask);
	virtual void Init();
	virtual void Init(const Vector2 & position, const float & rotation = 0.0f, const Vector2 & scale = Vector2(1.0f, 1.0f));
	template <typename T = std::enable_if<std::is_base_of<Component, T>::value>::type> void RemoveComponent(const unsigned int & id);
	void HandleMessage(const Message & message);
	void OnCollisionEnter(const CollisionData & data) override;
	void OnCollisionExit(const CollisionData & data) override;
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override;
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override;
	void SetCollisionFilter(const int & collisionCategory, const int & collisionMask);
	void SetCollisionCategory(const int & collisionCategory);
	int GetCollisionCategory();
	void SetCollisionMask(const int & collisionMask);
	int GetCollisionMask();
	const bool CollidesWith(const int & collisionMask);
	void Destroy();
	template <class Archive>
	void load(Archive & ar) {
		ar(cereal::make_nvp("Name", name),
		   cereal::make_nvp("Tag", tag),
		   cereal::make_nvp("Enabled", enabled),
		   cereal::make_nvp("CollisionCategory", collisionCategory),
		   cereal::make_nvp("CollisionMask", collisionMask),
		   cereal::make_nvp("ComponentManager", componentManager));
	}

	template <class Archive>
	void save(Archive & ar) const {
		ar(cereal::make_nvp("Type", std::string("GameObject")), 
			cereal::make_nvp("Name", name),
			cereal::make_nvp("Tag", tag),
			cereal::make_nvp("Enabled", enabled),
			cereal::make_nvp("CollisionCategory", collisionCategory),
			cereal::make_nvp("CollisionMask", collisionMask),
			cereal::make_nvp("ComponentManager", componentManager));
	}
protected:	
	std::weak_ptr<GameObject> GetWeak() { return shared_from_this(); }
	unsigned int objectID;
	unsigned int componentMask = 0;
	bool enabled = true;
	int collisionCategory = CATEGORY_ALL, collisionMask = 0xFFFF;
	ComponentManager componentManager;
	std::string name, tag = "default";
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
std::weak_ptr<T> GameObject::GetComponentInParent() {
	return componentManager.GetComponentInParent<T>();
}

template<typename T>
std::vector<std::weak_ptr<T>> GameObject::GetComponents() {
	return HasComponent<T>() ? componentManager.GetComponents<T>() : std::vector<std::weak_ptr<T>>();
}

template<typename T>
bool GameObject::ComponentExistsInParents() {
	return componentManager.ComponentExistsInParents<T>();
}

template<typename T>
bool GameObject::HasComponent() {
	if(componentMask == 0) return false;
	unsigned int component = TypeInfo::GetTypeID<T>();
	return (componentMask & component) == component;
}
template<typename T>
inline void GameObject::RemoveComponent(const unsigned int & id) {
	componentManager.RemoveComponent<T>(id);
}
#endif // !WS_GAMEOBJECT_H