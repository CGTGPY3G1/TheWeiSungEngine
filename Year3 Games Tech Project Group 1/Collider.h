#pragma once
#ifndef WS_COLLIDER_H
#define WS_COLLIDER_H
#include <Box2D\Dynamics\b2Fixture.h>
#include "Component.h"
#include "Physics.h"
struct PhysicsMaterial {
	float friction = 1.0f, restitution = 1.0f;
};
struct ColliderData {
	ComponentType type;
	std::weak_ptr<Component> comp;
};

class Collider : public Component {
public:
	Collider();
	Collider(std::weak_ptr<GameObject> gameObject);
	virtual ~Collider();
	friend class PhysicsSystem;
	b2Fixture * fixture;
	b2FixtureDef * fixtureDef;
	virtual Vector2 GetOffset() = 0;
	virtual void SetOffset(const Vector2 &  newOffset) = 0;
	bool IsSensor();
	void SetSensor(const bool & sensor);
protected:
	ColliderData * colliderData;
};

#endif // !WS_COLLIDER_H