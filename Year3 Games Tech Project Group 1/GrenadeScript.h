#pragma once
#ifndef WS_GRENADE_SCRIPT_H
#define WS_GRENADE_SCRIPT_H
#include "ScriptableComponent.h"

class RigidBody2D;
class Transform2D;
class GrenadeScript : public ScriptableComponent {
public:
	GrenadeScript();
	GrenadeScript(std::weak_ptr<GameObject> gameObject);
	~GrenadeScript();
	const ComponentType Type() const override { return COMPONENT_GRENADE_SCRIPT; }
	void Detonate();
	
	void Update(const float & deltaTime) override;
	const std::string GetName() const override { return "GrenadeScript"; }
	int GetSortOrder() override;
	void SetTimer(const float & time);
	const float GetBlastRadius() const;
	void SetBlastRadius(const float & radius);
	const float GetDamage() const;
	void SetDamage(const float & damage);
	void OnCollisionEnter(const CollisionData & data) override;
private:
	std::weak_ptr<Transform2D> transform;
	float lifeTime = 3.0f;
	bool alive = true;
	float radius = 5.0f, explosionForce = 100.0f, damage = 200.0f;
};

#endif // !WS_GRENADE_SCRIPT_H