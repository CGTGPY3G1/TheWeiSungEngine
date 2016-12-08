#pragma once
#ifndef WS_RIGIDBODY_2D_H
#define WS_RIGIDBODY_2D_H
#include <Box2D\Dynamics\b2Body.h>
#include "Component.h"
#include "TypeConversion.h"
#include "Physics.h"
enum ForceType {
	IMPULSE_FORCE = 0,
	FORCE = 1
};
enum SpaceType {
	WORLD_SPACE = 0,
	LOCAL_SPACE = 1,
	SAME_SPACE = 2
};

struct RigidBodyData {
	RigidBodyData(std::weak_ptr<Component> d) : data(d) {}
	std::weak_ptr<Component> data;
};
class RigidBody2D : public Component {
public:
	RigidBody2D(std::weak_ptr<GameObject> gameObject);
	~RigidBody2D();
	const ComponentType Type() const override { return COMPONENT_RIGID_BODY_2D; }
	void AddForce(const Vector2 & force, const ForceType & forceType = FORCE);
	void AddForceAtPoint(const Vector2 & force, const Vector2 & point, const ForceType & forceType = FORCE);
	void AddTorque(const float & force, const ForceType & forceType = FORCE);
	void SetAngularVelocity(const float & velocity);
	float GetAngularVelocity();
	float GetInertia();
	void OffsetCentre(const Vector2 & offset);
	Vector2 GetPosition();
	void SetPosition(const Vector2 & newPosition);
	void SetRotation(const float & angle);
	Vector2 GetForward();
	Vector2 GetRight();
	float GetRotation();
	void SetMass(const float & newMass);
	float GetMass();
	Vector2 GetVelocity();
	void SetVelocity(const Vector2 & newVelocity);
	float GetSpeed();
	void Init(const b2BodyType & type = b2BodyType::b2_kinematicBody, const float & angularDampening = 0.05f, const float & linearDampening = 1.0f);
	b2Body * GetBody();
	b2BodyDef * GetBodyDef();
private:
	friend class PhysicsSystem;
	friend class Collider;
	b2Body * body;
	b2BodyDef * bodyDef;
	b2MassData * massData;
	RigidBodyData * rigidBodyData;
};

#endif // !WS_RIGIDBODY_2D_H