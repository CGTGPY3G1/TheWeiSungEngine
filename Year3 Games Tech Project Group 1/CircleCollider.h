#pragma once
#ifndef WS_CIRCLE_COLLIDER_H
#define WS_CIRCLE_COLLIDER_H
#include <Box2D\Collision\Shapes\b2CircleShape.h>
#include "Collider.h"
class CircleCollider : public Collider {
public:
	CircleCollider();
	CircleCollider(std::weak_ptr<GameObject> gameObject);
	~CircleCollider();
	const ComponentType Type() const override { return COMPONENT_CIRCLE_COLLIDER; }
	void Init(const Vector2 & position, const float & radius, const bool & isSensor = false, const float & friction = 1.0f, const float & restitution = 1.0f);
	b2CircleShape *shape;
	
};
#endif // !WS_CIRCLE_COLLIDER_H 