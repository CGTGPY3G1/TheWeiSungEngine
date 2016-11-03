#ifndef WS_COMPONENTS_H
#define WS_COMPONENTS_H
enum ComponentType : unsigned int {
	COMPONENT_NULL = 0,
	COMPONENT_TRANSFORM_2D = 1 << 0,
	COMPONENT_SPRITE_RENDERER = 1 << 1,
	COMPONENT_RIGID_BODY_2D = 1 << 2,
	COMPONENT_CIRCLE_COLLIDER = 1 << 3,
	COMPONENT_BOX_COLLIDER_2D = 1 << 4,
	COMPONENT_POLYGON_COLLIDER_2D = 1 << 5
};

#define NUMBER_OF_COMPONENTS = 6
#include <memory>
#include <list>
#include <string>
#include "Message.h"
#include "Vector2.h"
class IDManager {
public:
	~IDManager() {}
	static int getNewCompID();
private:
	static int compCount;
};


class GameObject;
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
	template<typename T> std::weak_ptr<T> AddComponent();
	template<typename T> std::weak_ptr<T> GetComponent();
	template<typename T> std::list<std::weak_ptr<T>> GetComponents();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
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
	//virtual void initialize() = 0;
protected:
	std::weak_ptr<Component> GetWeak() { return shared_from_this(); }
	bool enabled = false;
	std::string name;
	unsigned int ownerID, compID;
	void Init(const bool & enabled, const std::string & name);
	std::weak_ptr<GameObject> gameObject;
};



#endif // !WS_COMPONENTS_H

