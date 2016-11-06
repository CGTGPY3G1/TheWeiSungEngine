#pragma once
#ifndef WS_SCRIPTABLE_COMPONENT_H
#define WS_SCRIPTABLE_COMPONENT_H
#include "Component.h"
#include "CollisionHandler.h"

class ScriptableComponent : public Component, public CollisionHandler {
public:
	ScriptableComponent(std::weak_ptr<GameObject> gameObject);
	virtual ~ScriptableComponent() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	void OnCollisionEnter(const CollisionData & data) override {}
	void OnCollisionExit(const CollisionData & data) override {}
};


#endif // !WS_SCRIPTABLE_COMPONENT_H
