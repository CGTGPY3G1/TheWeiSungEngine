#pragma once
#ifndef WS_PHYSICS_SYSTEM_H
#define WS_PHYSICS_SYSTEM_H
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2World.h>
#include <Box2D\Dynamics\Contacts\b2Contact.h>
#include "Message.h"
#include "Vector2.h"
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

class b2World;
class b2Body;
class Scene;
class PhysicsSystem : public b2ContactListener {
public:
	PhysicsSystem(Scene * owner);
	~PhysicsSystem();
	void Update(const float & deltaTime);
	b2Body * CreateBody(const Vector2 & position, const PhysicsBodyType & type);
	void CreateBox(const float & x, const float & y, const float & width, const float & height);
	void SetGravity(const float & x, const float & y);
	void BeginContact(b2Contact* contact) {
		//std::weak_ptr<Collider> r1 = (std::weak_ptr<Collider>) contact->GetFixtureA()->GetBody()->GetUserData();
		//std::weak_ptr<Collider> r2 = (std::weak_ptr<Collider>) contact->GetFixtureB()->GetBody()->GetUserData();
		///*if(a != nullptr && a->getOwner() != nullptr) {
		//	a->beginCollision(b, contact);
		//}
		//if(b != nullptr && b->getOwner() != nullptr) {
		//	b->beginCollision(a, contact);
		//}*/
	}

	void EndContact(b2Contact* contact) {
		//std::shared_ptr<RigidBody2D> r1 = (std::shared_ptr<RigidBody2D>) contact->GetFixtureA()->GetBody()->GetUserData();
		//std::shared_ptr<RigidBody2D> r2 = (std::shared_ptr<RigidBody2D>) contact->GetFixtureB()->GetBody()->GetUserData();
		///*if(a != nullptr && a->getOwner() != nullptr) {
		//	a->endCollision(b, contact);
		//}
		//if(b != nullptr && b->getOwner() != nullptr) {
		//	b->endCollision(a, contact);
		//}*/
	}
	void HandleMessage(const Message & message);
	void Draw();
private:
	b2World * world;
	PhysicsSettings settings;
	float accumulator = 0.0f;
	Scene * scene;
};



#endif // !WS_PHYSICS_SYSTEM_H
