#pragma once
#ifndef WS_CHARACTER_MOVEMENT_SCRIPT_H
#define WS_CHARACTER_MOVEMENT_SCRIPT_H
#include "ScriptableComponent.h"
class RigidBody2D;
class Transform2D;
class CharacterMovementScript : public ScriptableComponent{
public:
	CharacterMovementScript(std::weak_ptr<GameObject> gameObject);
	~CharacterMovementScript();
	const ComponentType Type() const override { return COMPONENT_CHARACTER_MOVEMENT; }
	void Start() override;
	void MoveUsingPhysics(Vector2 & force, const bool & worldSpace = true);
	void Move(Vector2 & amount, const bool & worldSpace = true);
	//void Move(const Vector2 & direction, const float & scale);
//	void MoveLocal(const Vector2 & direction, const float & scale);
//	void MoveLocal(const Vector2 & amount);
private:
	std::weak_ptr<RigidBody2D> rigidbody;
	std::weak_ptr<Transform2D> transform;
};


#endif // !WS_CHARACTER_MOVEMENT_SCRIPT_H
