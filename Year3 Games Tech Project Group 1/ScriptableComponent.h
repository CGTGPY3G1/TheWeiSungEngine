#pragma once
#ifndef WS_SCRIPTABLE_COMPONENT_H
#define WS_SCRIPTABLE_COMPONENT_H
#include "Component.h"
#include "CollisionHandler.h"

class ScriptableComponent : public Component, public CollisionHandler {
public:
	ScriptableComponent(std::weak_ptr<GameObject> gameObject);
	virtual ~ScriptableComponent() {}
	const ComponentType Type() const override { return COMPONENT_SCRIPTABLE; }
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	void OnCollisionEnter(const CollisionData & data) override {}
	void OnCollisionExit(const CollisionData & data) override {}
	void OnSensorEnter(const std::weak_ptr<Collider> & collider) override {}
	void OnSensorExit(const std::weak_ptr<Collider> & collider) override {}
	std::weak_ptr<ScriptableComponent> GetWeak() { return std::static_pointer_cast<ScriptableComponent>(shared_from_this()); }
};


#endif // !WS_SCRIPTABLE_COMPONENT_H
