#pragma once
#ifndef WS_COLLISION_HANDLER_H
#define WS_COLLISION_HANDLER_H
#include "CollisionData.h"

class CollisionHandler {
public:
	virtual ~CollisionHandler() {}
	virtual void OnCollisionEnter(const CollisionData & data) = 0;
	virtual void OnCollisionExit(const CollisionData & data) = 0;
	virtual void OnSensorEnter(const std::weak_ptr<Collider> & collider) = 0;
	virtual void OnSensorExit(const std::weak_ptr<Collider> & collider) = 0;
};

#endif // !WS_COLLISION_HANDLER_H
