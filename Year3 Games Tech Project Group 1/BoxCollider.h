#pragma once
#ifndef WS_BOX_COLLIDER_H
#define WS_BOX_COLLIDER_H
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include "Collider.h"

class BoxCollider : public Collider {
public:
	BoxCollider();
	BoxCollider(std::weak_ptr<GameObject> gameObject);
	~BoxCollider();
	const ComponentType Type() const override { return COMPONENT_BOX_COLLIDER_2D; }
	void Init(const Vector2 & centre, const Vector2 & size, const bool & isSensor = false, const float & friction = 1.0f, const float & restitution = 1.0f);
	void SetRotation(const float & angle);
	Vector2 GetOffset() override;
	void SetOffset(const Vector2 & newOffset) override;
	b2PolygonShape* shape;
private:
	Vector2 offset;
	float rotation = 0;
};
#endif // !WS_BOX_COLLIDER_H
 