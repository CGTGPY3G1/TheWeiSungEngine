#pragma once
#ifndef WS_BULLET_SCRIPT_H
#define WS_BULLET_SCRIPT_H
#include "ScriptableComponent.h"
#include "cereal\cereal.hpp"
#include "cereal\access.hpp"
#include "cereal\details\traits.hpp"
#include <cereal\types\polymorphic.hpp>
class RigidBody2D;
class Transform2D;
class BulletScript : public ScriptableComponent {
public:
	BulletScript();
	BulletScript(std::weak_ptr<GameObject> gameObject);
	~BulletScript();
	const ComponentType Type() const override { return COMPONENT_BULLET_SCRIPT; }
	void Start() override;
	void Update(const float & deltaTime) override;
	const std::string GetName() const override { return "BulletScript"; }
	void OnCollisionEnter(const CollisionData & data) override;
	int GetSortOrder() override;
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		ScriptableComponent::save(ar);
	}
private:
	float lifeTime = 3.5f;
	bool alive = true;
};

#endif // !WS_BULLET_SCRIPT_H