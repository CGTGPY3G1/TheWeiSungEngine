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
	void Init(const Vector2 & position, const float & radius, const bool & isSensor = false, const float & density = 1.0f, const float & friction = 0.4f, const float & restitution = 0.0f);
	Vector2 GetOffset() override;
	void SetOffset(const Vector2 & newOffset) override;
	b2CircleShape *shape;
	
};
#endif // !WS_CIRCLE_COLLIDER_H 