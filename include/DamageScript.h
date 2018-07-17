#pragma once
#ifndef WS_DAMAGE_SCRIPT_H
#define WS_DAMAGE_SCRIPT_H
#include "ScriptableComponent.h"
class RigidBody2D;
class DamageScript : public ScriptableComponent {
public:
	DamageScript();
	DamageScript(std::weak_ptr<GameObject> gameObject);
	~DamageScript();
	const ComponentType Type() const override { return COMPONENT_DAMAGE_SCRIPT; }
	void Start() override;
	const std::string GetName() const override { return "DamageScript"; }
	int GetSortOrder() override;
	void OnCollisionEnter(const CollisionData & data) override;
private:
	std::weak_ptr<RigidBody2D> rigidBody;
};

#endif // !WS_DAMAGE_SCRIPT_H