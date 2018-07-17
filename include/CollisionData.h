#pragma once
#ifndef WS_COLLISION_DATA_H
#define WS_COLLISION_DATA_H

#include "Vector2.h"
#include "Collider.h"

struct ContactPoint {
	Vector2 point;
	float seperation;
};

struct CollisionData {
	CollisionData(const std::weak_ptr<GameObject> & otherGameObject, const std::weak_ptr<Collider> & otherFixture, const Vector2 & cNormal, const Vector2 & velocity);
	std::weak_ptr<GameObject> gameObject;
	std::weak_ptr<Collider> fixture;
	Vector2 normal, relativeVelocity;
	ContactPoint contactPoints[2];
};


#endif // !WS_COLLISION_DATA_H