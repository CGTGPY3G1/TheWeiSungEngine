#pragma once
#ifndef WS_CHARACTER_MOVEMENT_SCRIPT_H
#define WS_CHARACTER_MOVEMENT_SCRIPT_H
#include "ScriptableComponent.h"
#include "cereal\cereal.hpp"
#include "cereal\access.hpp"
#include "cereal\details\traits.hpp"
#include <cereal\types\polymorphic.hpp>
class RigidBody2D;
class Transform2D;
class CharacterMovementScript : public ScriptableComponent{
public:
	CharacterMovementScript();
	CharacterMovementScript(std::weak_ptr<GameObject> gameObject);
	~CharacterMovementScript();
	const ComponentType Type() const override { return COMPONENT_CHARACTER_MOVEMENT; }
	void Start() override;
	void MoveUsingPhysics(Vector2 & force, const bool & worldSpace = true);
	void Move(Vector2 & amount, const bool & worldSpace = true);
	const std::string GetName() const override { return "CharacterMovementScript"; }
	int GetSortOrder() override;
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		ScriptableComponent::save(ar);
	}
private:
	std::weak_ptr<RigidBody2D> rigidbody;
	std::weak_ptr<Transform2D> transform;
};


#endif // !WS_CHARACTER_MOVEMENT_SCRIPT_H
