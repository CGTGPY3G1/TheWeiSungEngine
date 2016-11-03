#pragma once
#ifndef WS_POLYGON_COLLIDER_H
#define WS_POLYGON_COLLIDER_H
#include <Box2D\Collision\Shapes\b2PolygonShape.h>
#include "Collider.h"
#include <initializer_list>
class PolygonCollider : public Collider {
public:
	PolygonCollider();
	PolygonCollider(std::weak_ptr<GameObject> gameObject);
	~PolygonCollider();
	const ComponentType Type() const override { return COMPONENT_POLYGON_COLLIDER_2D; }
	void Init(const Vector2 & center, std::initializer_list<Vector2> verts, const bool & isSensor = false, const float & friction = 1.0f, const float & restitution = 1.0f);
	Vector2 GetOffset() override;
	void SetOffset(const Vector2 & newOffset) override;
private:
	Vector2 offset;
	b2PolygonShape* shape;
};

#endif // !WS_POLYGON_COLLIDER_H 