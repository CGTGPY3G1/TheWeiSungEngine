#pragma once
#ifndef WS_PHYSICS_SYSTEM_H
#define WS_PHYSICS_SYSTEM_H
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Dynamics\Contacts\b2Contact.h>
#include "Message.h"

enum PhysicsBodyType {
	STATIC_BODY = 0,
	KINEMATIC_BODY = 1,
	DYNAMIC_BODY = 2
};

struct PhysicsSettings {
	b2Vec2 gravity = {0, 0};
	float timeStep = 1.0f / 50.0f;
	int velocityIterations = 8, positionIterations = 3;
};
class Vector2;
class b2World;
class b2Body;
class PhysicsSystem : public b2ContactListener {
public:
	PhysicsSystem();
	~PhysicsSystem();
	void Update(const float & deltaTime);
	void UpdateBodies();
	b2Body * CreateBody(const Vector2 & position, const PhysicsBodyType & type);
	void CreateBox(const float & x, const float & y, const float & width, const float & height);
	void SetGravity(const float & x, const float & y);
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void HandleMessage(const Message & message);
	void Draw();
private:
	b2World * world;
	PhysicsSettings settings;
	float accumulator = 0.0f;
};

#endif // !WS_PHYSICS_SYSTEM_H
