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
	void Init(const Vector2 & centre, const Vector2 & size, const bool & isSensor = false, const float & density = 1.0f, const float & friction = 0.4f, const float & restitution = 0.0f);
	void SetRotation(const float & angle);
	Vector2 GetOffset() override;
	void SetOffset(const Vector2 & newOffset) override;
	b2PolygonShape* shape;
	const std::string GetName() const override { return "BoxCollider"; }
	template <class Archive>
	void load(Archive & ar) {
		ar(cereal::make_nvp("Offset", offset),
		   cereal::make_nvp("Width", size));
	}

	template <class Archive>
	void save(Archive & ar) const {
		Collider::save(ar);
		ar(cereal::make_nvp("Offset", offset),
		   cereal::make_nvp("Width", size));
	}
private:
	Vector2 offset, size;
	float rotation = 0;
};
#endif // !WS_BOX_COLLIDER_H
  