#pragma once
#ifndef WS_PHYSICS_SYSTEM_H
#define WS_PHYSICS_SYSTEM_H
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Dynamics\Contacts\b2Contact.h>
#include "Message.h"
#include <vector>
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

class Collider;
struct RayCastHit {
	int hits = 0;
	std::vector<std::weak_ptr<Collider>> colliders;
	void Reset() { hits = 0; colliders.clear(); }
};
class Vector2;
class b2World;
class b2Body;
class PhysicsSystem : public b2ContactListener, public b2RayCastCallback{
public:
	static PhysicsSystem & GetInstance();
	~PhysicsSystem();
	void Update(const float & deltaTime);
	void UpdateBodies();
	b2Body * CreateBody(const Vector2 & position, const PhysicsBodyType & type);
	void CreateBox(const float & x, const float & y, const float & width, const float & height);
	void SetGravity(const float & x, const float & y);
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	RayCastHit RayCast(const Vector2 & start, const Vector2 & end);
	void HandleMessage(const Message & message);
	void Draw();
private:
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	PhysicsSystem();
	b2World * world;
	PhysicsSettings settings;
	float accumulator = 0.0f;
	RayCastHit hit;
	bool dirty = false;
};

#endif // !WS_PHYSICS_SYSTEM_H
