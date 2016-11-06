#include "PhysicsSystem.h"
#include "Physics.h"
#include "Engine.h"
#include "DebugDraw.h"
#include "Math.h"
#include "Vector2.h"

#include "GameObject.h"
PhysicsSystem::PhysicsSystem(){
	world = new b2World(b2Vec2(0, 0));
	world->SetAllowSleeping(false);
	world->SetDebugDraw(Engine::GetInstance().GetDebugDraw());
	world->SetContactListener(this);
}

PhysicsSystem::~PhysicsSystem() {
}

void stop() {

}

void PhysicsSystem::Update(const float & deltaTime) {
	accumulator += deltaTime;
	bool updateBodies = false;
	while(accumulator >= settings.timeStep) {
		if(!updateBodies) updateBodies = true;
		world->Step(settings.timeStep, settings.velocityIterations, settings.positionIterations);
		accumulator -= settings.timeStep;
	}
	if(updateBodies) {
		for(b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
			if(b->GetType() == b2BodyType::b2_dynamicBody) {
				RigidBodyData * rb = (RigidBodyData *)b->GetUserData();
				if(rb) {
					std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(rb->data.lock());
					std::shared_ptr<Transform2D> t = r->GetComponent<Transform2D>().lock();
					t->SetPosition(TypeConversion::ConvertToVector2(b->GetPosition()));
					t->SetRotation(b->GetAngle()*Math::RadiansToDegrees());
				}
			}
			else if(b->GetType() == b2BodyType::b2_kinematicBody) {
				RigidBodyData * rb = (RigidBodyData *)b->GetUserData();
				if(rb) {
					std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(rb->data.lock());
					std::shared_ptr<Transform2D> t = r->GetComponent<Transform2D>().lock();
					b2Vec2 newPos = TypeConversion::ConvertToB2Vector2(t->GetPosition());
					float newAngle = t->GetRotation() * Math::DegreesToRadians();
					b->SetTransform(newPos, newAngle);
					b->SetMassData(r->massData);
				}
			}
		}
	}
}


b2Body * PhysicsSystem::CreateBody(const Vector2 & position, const PhysicsBodyType & type) {
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(position.x * Physics::METRES_PER_PIXEL, position.y * Physics::METRES_PER_PIXEL);
	bodyDef.type = (b2BodyType)type;
	b2Body* body = world->CreateBody(&bodyDef);
	return body;
}

void PhysicsSystem::CreateBox(const float & x, const float & y, const float & width, const float & height) {
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(x * Physics::METRES_PER_PIXEL, y * Physics::METRES_PER_PIXEL);
	bodyDef.type = b2_dynamicBody;
	b2Body* Body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width / 2) * Physics::METRES_PER_PIXEL, (height / 2) * Physics::METRES_PER_PIXEL);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &shape;
	Body->CreateFixture(&FixtureDef);
}


void PhysicsSystem::SetGravity(const float & x, const float & y) {
	world->SetGravity(b2Vec2(x, y));
}

void PhysicsSystem::BeginContact(b2Contact* contact) {
	b2Fixture * fixture1 = contact->GetFixtureA(), *fixture2 = contact->GetFixtureB();
	
	b2Body * body1 = fixture1->GetBody(), *body2 = fixture2->GetBody();
	ColliderData * colliderData1 = (ColliderData *)fixture1->GetUserData();
	ColliderData * colliderData2 = (ColliderData *)fixture2->GetUserData();
	if(colliderData1 && colliderData2) {
		std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
		std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
		std::weak_ptr<GameObject> gameObject1 = collider1.lock()->GetGameObject();
		std::weak_ptr<GameObject> gameObject2 = collider2.lock()->GetGameObject();
		if(fixture2->IsSensor() || fixture1->IsSensor()) {
			gameObject1.lock()->OnSensorEnter(collider2);
			gameObject2.lock()->OnSensorEnter(collider1);
		}
		else {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			b2Vec2 vel1 = body1->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
			b2Vec2 vel2 = body2->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
			Vector2 relativeVelocity = TypeConversion::ConvertToVector2(vel2 - vel1);
			b2Vec2 normalStart = worldManifold.points[0] + worldManifold.normal;
			b2Vec2 normalEnd = worldManifold.points[0] - worldManifold.normal;
			Vector2 normal = TypeConversion::ConvertToVector2((normalEnd - normalStart)).Normalize();
			const size_t cps = 2;
			ContactPoint contactPoints[2];
			for(size_t i = 0; i < cps; i++) {
				contactPoints[i].point = TypeConversion::ConvertToVector2(worldManifold.points[i]);
				contactPoints[i].seperation = worldManifold.separations[i] * Physics::PIXELS_PER_METRE;
			}
			{
				CollisionData collisionData = CollisionData(gameObject2, collider2, normal, relativeVelocity);
				for(size_t i = 0; i < cps; i++) {
					collisionData.contactPoints[i] = contactPoints[i];
				}
				gameObject1.lock()->OnCollisionEnter(collisionData);
			}
			{
				CollisionData collisionData = CollisionData(gameObject1, collider1, -normal, -relativeVelocity);
				for(size_t i = 0; i < cps; i++) {
					collisionData.contactPoints[i] = contactPoints[i];
				}
				gameObject2.lock()->OnCollisionEnter(collisionData);
			}
		}	
	}
}

void PhysicsSystem::EndContact(b2Contact* contact) {
	b2Fixture * fixture1 = contact->GetFixtureA(), *fixture2 = contact->GetFixtureB();
	b2Body * body1 = fixture1->GetBody(), *body2 = fixture2->GetBody();
	ColliderData * colliderData1 = (ColliderData *)fixture1->GetUserData();
	ColliderData * colliderData2 = (ColliderData *)fixture2->GetUserData();
	if(colliderData1 && colliderData2) {
		std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
		std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
		std::weak_ptr<GameObject> gameObject1 = collider1.lock()->GetGameObject();
		std::weak_ptr<GameObject> gameObject2 = collider2.lock()->GetGameObject();
		if(fixture1->IsSensor() || fixture2->IsSensor()) {
			gameObject1.lock()->OnSensorExit(collider2);
			gameObject2.lock()->OnSensorExit(collider1);
		}
		else {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			b2Vec2 vel1 = body1->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
			b2Vec2 vel2 = body2->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
			Vector2 impactVelocity = TypeConversion::ConvertToVector2(vel1 - vel2);
			b2Vec2 normalStart = worldManifold.points[0] + worldManifold.normal;
			b2Vec2 normalEnd = worldManifold.points[0] - worldManifold.normal;
			Vector2 normal = TypeConversion::ConvertToVector2((normalEnd - normalStart)).Normalize();

			const size_t cps = 2;
			ContactPoint contactPoints[2];
			for(size_t i = 0; i < cps; i++) {
				contactPoints[i].point = TypeConversion::ConvertToVector2(worldManifold.points[i]);
				contactPoints[i].seperation = worldManifold.separations[i] * Physics::PIXELS_PER_METRE;
			}
			{
				CollisionData collisionData(gameObject2, collider2, normal, impactVelocity);
				for(size_t i = 0; i < cps; i++) {
					collisionData.contactPoints[i] = contactPoints[i];
				}
				gameObject1.lock()->OnCollisionExit(collisionData);
			}
			{
				CollisionData collisionData(gameObject1, collider1, -normal, -impactVelocity);
				for(size_t i = 0; i < cps; i++) {
					collisionData.contactPoints[i] = contactPoints[i];
				}
				gameObject2.lock()->OnCollisionExit(collisionData);
			}
		}
		
	}
}

void PhysicsSystem::HandleMessage(const Message & message) {
	MessageType type = message.type;
	switch(message.type) {
	case MessageType::MESSAGE_TYPE_VOID:
		break;
	case MessageType::MESSAGE_TYPE_REGISTER_RIGIDBODY:
	{
		RigidBodyData *data = (RigidBodyData*)message.data;
		std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(data->data.lock());
		r->body = world->CreateBody(r->bodyDef);
		break;
	}
	case MessageType::MESSAGE_TYPE_UNREGISTER_RIGIDBODY:
	{
		RigidBodyData *data = (RigidBodyData*)message.data;
		std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(data->data.lock());
		world->DestroyBody(r->body);
		break;
	}
	case MessageType::MESSAGE_TYPE_REGISTER_COLLIDER:
	{
		ColliderData *data = (ColliderData*)message.data;
		if(data->type == ComponentType::COMPONENT_BOX_COLLIDER_2D) {
			std::shared_ptr<BoxCollider> b = std::static_pointer_cast<BoxCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = b->GetComponent<RigidBody2D>().lock();
			b->fixture = r->body->CreateFixture(b->fixtureDef);
		}
		else if(data->type == ComponentType::COMPONENT_CIRCLE_COLLIDER) {
			std::shared_ptr<CircleCollider> c = std::static_pointer_cast<CircleCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = c->GetComponent<RigidBody2D>().lock();
			c->fixture = r->body->CreateFixture(c->fixtureDef);
		}
		else if(data->type == ComponentType::COMPONENT_POLYGON_COLLIDER_2D) {
			std::shared_ptr<PolygonCollider> p = std::static_pointer_cast<PolygonCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = p->GetComponent<RigidBody2D>().lock();
			p->fixture = r->body->CreateFixture(p->fixtureDef);
		}
		break;
	}
	case MessageType::MESSAGE_TYPE_UNREGISTER_COLLIDER:
	{
		ColliderData *data = (ColliderData*)message.data;
		if(data->type == ComponentType::COMPONENT_BOX_COLLIDER_2D) {
			std::shared_ptr<BoxCollider> b = std::static_pointer_cast<BoxCollider>(data->comp.lock());
			b->GetComponent<RigidBody2D>().lock()->body->DestroyFixture(b->fixture);
		}
		else if(data->type == ComponentType::COMPONENT_CIRCLE_COLLIDER) {
			std::shared_ptr<CircleCollider> c = std::static_pointer_cast<CircleCollider>(data->comp.lock());
			c->GetComponent<RigidBody2D>().lock()->body->DestroyFixture(c->fixture);
		}
		else if(data->type == ComponentType::COMPONENT_POLYGON_COLLIDER_2D) {
			std::shared_ptr<PolygonCollider> p = std::static_pointer_cast<PolygonCollider>(data->comp.lock());
			p->GetComponent<RigidBody2D>().lock()->body->DestroyFixture(p->fixture);
		}
		break;
	}
	default:
		break;
	}
}

void PhysicsSystem::Draw() {
	world->DrawDebugData();
}
