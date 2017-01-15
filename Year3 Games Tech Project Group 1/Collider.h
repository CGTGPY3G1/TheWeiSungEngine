#pragma once
#ifndef WS_COLLIDER_H
#define WS_COLLIDER_H
#include <Box2D\Dynamics\b2Fixture.h>
#include "Component.h"
#include "Physics.h"
#include <cereal/types/base_class.hpp>
struct PhysicsMaterial {
	float friction = 1.0f, restitution = 1.0f;
};
struct ComponentData;
class Collider : public Component {
	friend class PhysicsSystem;
public:
	Collider();
	Collider(std::weak_ptr<GameObject> gameObject);
	virtual ~Collider();
	virtual Vector2 GetOffset() { return Vector2(); }
	virtual void SetOffset(const Vector2 &  newOffset) {}
	bool IsSensor();
	void SetSensor(const bool & sensor);
	void SetCollisionFilter(const int & collisionCategory, const int & collisionMask);
	void SetCollisionCategory(const int & collisionCategory);
	int GetCollisionCategory();
	void SetCollisionMask(const int & collisionMask);
	int GetCollisionMask();


	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		Component::save(ar);
		ar(cereal::make_nvp("Density", density),
		   cereal::make_nvp("Restitution", restitution),
		   cereal::make_nvp("Friction", friction),
		   cereal::make_nvp("IsSensor", (bool)isSensor));
	}
protected:
	void UpdateCollisionFilter();
	b2Fixture * fixture;
	b2FixtureDef * fixtureDef;
	float density, restitution, friction;
	bool isSensor;
};

#endif // !WS_COLLIDER_H