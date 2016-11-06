#pragma once
#ifndef WS_COLLISION_HANDLER_H
#define WS_COLLISION_HANDLER_H
#include "CollisionData.h"

class CollisionHandler {
public:
	virtual ~CollisionHandler() {}
	virtual void OnCollisionEnter(const CollisionData & data) = 0;
	virtual void OnCollisionExit(const CollisionData & data) = 0;
};

#endif // !WS_COLLISION_HANDLER_H
