#pragma once
#ifndef WS_PHYSICS_SYSTEM_H
#define WS_PHYSICS_SYSTEM_H
#include "CollisionMask.h"
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Dynamics\Contacts\b2Contact.h>
#include "Message.h"
#include <vector>
#include "Vector2.h"
#include "AABB.h"
#include <limits>
enum PhysicsBodyType {
	STATIC_BODY = 0,
	KINEMATIC_BODY = 1,
	DYNAMIC_BODY = 2
};

struct PhysicsSettings {
	b2Vec2 gravity = {0, 0};
	float timeStep = 1.0f / 50.0f;
	int velocityIterations = 6, positionIterations = 3;
};

class Collider;
struct RayCastHit {
	bool hit = false;
	std::weak_ptr<Collider> collider;
	Vector2 normal, point;
	float fraction = std::numeric_limits<float>::max();
	void Reset() { 
		hit = false; collider = std::weak_ptr<Collider>(); normal = Vector2::Up; point = Vector2::Zero; fraction = std::numeric_limits<float>::max();
	}
};

//struct AABB;
class b2World;
class b2Body;
class PhysicsSystem : public b2ContactListener, public b2RayCastCallback, public b2QueryCallback, public b2ContactFilter {
public:
	static PhysicsSystem & GetInstance();
	~PhysicsSystem();
	void Update(const float & deltaTime);
	void UpdateBodies();
	b2Body * CreateBody(const Vector2 & position, const PhysicsBodyType & type);
	void CreateBox(const float & x, const float & y, const float & width, const float & height);
	void SetGravity(const float & x, const float & y);
	void BeginContact(b2Contact* contact);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	void EndContact(b2Contact* contact);
	RayCastHit RayCast(const Vector2 & start, const Vector2 & end, const bool & reportSensors = false, const int & collisionMask = (int)CollisionCategory::CATEGORY_ALL);
	std::vector<std::weak_ptr<Collider>> CircleCast(const Vector2 & position, const float & radius, const bool & reportSensors = false, const int & collisionMask = (int)CollisionCategory::CATEGORY_ALL);
	bool CheckAABB(const AABB & aabb, const int & collisionMask = (int)CollisionCategory::CATEGORY_ALL);
	void HandleMessage(const Message & message);
	void Draw();
private:
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	bool ReportFixture(b2Fixture* fixture);
	PhysicsSystem();
	b2World * world;
	std::vector<std::weak_ptr<Collider>> aabbHits;
	PhysicsSettings settings;
	float accumulator = 0.0f;
	RayCastHit hit;
	bool dirty = false, reportSensors = true;
	int raycastFilter, aabbFilter;
	bool aabbHit = false, checkMultipleAABBs = false;
};

#endif // !WS_PHYSICS_SYSTEM_H
