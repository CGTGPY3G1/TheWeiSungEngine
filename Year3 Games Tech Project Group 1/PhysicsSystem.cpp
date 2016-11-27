#include "PhysicsSystem.h"
#include "Physics.h"
#include "Engine.h"
#include "DebugDraw.h"
#include "Math.h"
#include "Vector2.h"
#include "GameObject.h"
#include "ComponentData.h"
#include "CollisionData.h"
PhysicsSystem::PhysicsSystem(){
	world = new b2World(b2Vec2(0, 0));
	world->SetAllowSleeping(false);
	DebugDraw * debugDraw = Engine::GetInstance().GetDebugDraw();
	debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
	world->SetDebugDraw(debugDraw);
	world->SetContactListener(this);
}

PhysicsSystem::~PhysicsSystem() {
}

void stop() {

}

void PhysicsSystem::Update(const float & deltaTime) {
	world->Step(deltaTime, settings.velocityIterations, settings.positionIterations);
}

void PhysicsSystem::UpdateBodies() {
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if(b->GetType() == b2BodyType::b2_dynamicBody) {
			RigidBodyData * rb = (RigidBodyData *)b->GetUserData();
			if(rb) {
				std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(rb->data.lock());
				std::shared_ptr<Transform2D> t = r->GetComponent<Transform2D>().lock();
				t->SetPosition(TypeConversion::ConvertToVector2(b->GetPosition()));
				t->SetRotation(b->GetAngle() * Math::RadiansToDegrees());
			}
		}
		else if(b->GetType() == b2BodyType::b2_kinematicBody || b->GetType() == b2BodyType::b2_staticBody) {
			RigidBodyData * rb = (RigidBodyData *)b->GetUserData();
			if(rb) {
				std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(rb->data.lock());
				std::shared_ptr<Transform2D> t = r->GetComponent<Transform2D>().lock();
				b2Vec2 newPos = TypeConversion::ConvertToB2Vector2(t->GetPosition());
				float newAngle = t->GetRotation() * Math::DegreesToRadians();
				b->SetTransform(newPos, newAngle);
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
	ComponentData * colliderData1 = (ComponentData *)fixture1->GetUserData();
	ComponentData * colliderData2 = (ComponentData *)fixture2->GetUserData();
	if(colliderData1 && colliderData2) {
		std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
		std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
		RigidBodyData * rigidBodyData1 = (RigidBodyData *)body1->GetUserData();
		RigidBodyData * rigidBodyData2 = (RigidBodyData *)body2->GetUserData();
		std::weak_ptr<RigidBody2D> rigidBody1 = std::static_pointer_cast<RigidBody2D>(rigidBodyData1->data.lock());
		std::weak_ptr<RigidBody2D> rigidBody2 = std::static_pointer_cast<RigidBody2D>(rigidBodyData2->data.lock());
		std::weak_ptr<GameObject> gameObject1 = rigidBody1.lock()->GetGameObject();
		std::weak_ptr<GameObject> gameObject2 = rigidBody2.lock()->GetGameObject();
		if(fixture2->IsSensor() || fixture1->IsSensor()) {
			gameObject1.lock()->OnSensorEnter(collider2);
			gameObject2.lock()->OnSensorEnter(collider1);
		}
		else {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			Vector2 impactVelocity = rigidBody2.lock()->GetVelocity();
			impactVelocity -= rigidBody1.lock()->GetVelocity();
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
				CollisionData collisionData = CollisionData(gameObject2, collider2, normal, impactVelocity);
				for(size_t i = 0; i < cps; i++) {
					collisionData.contactPoints[i] = contactPoints[i];
				}
				gameObject1.lock()->OnCollisionEnter(collisionData);
			}
			{
				CollisionData collisionData = CollisionData(gameObject1, collider1, -normal, -impactVelocity);
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
	ComponentData * colliderData1 = (ComponentData *)fixture1->GetUserData();
	ComponentData * colliderData2 = (ComponentData *)fixture2->GetUserData();
	if(colliderData1 && colliderData2) {
		std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
		std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
		RigidBodyData * rigidBodyData1 = (RigidBodyData *)body1->GetUserData();
		RigidBodyData * rigidBodyData2 = (RigidBodyData *)body2->GetUserData();
		std::weak_ptr<RigidBody2D> rigidBody1 = std::static_pointer_cast<RigidBody2D>(rigidBodyData1->data.lock());
		std::weak_ptr<RigidBody2D> rigidBody2 = std::static_pointer_cast<RigidBody2D>(rigidBodyData2->data.lock());
		std::weak_ptr<GameObject> gameObject1 = rigidBody1.lock()->GetGameObject();
		std::weak_ptr<GameObject> gameObject2 = rigidBody2.lock()->GetGameObject();
		if(fixture1->IsSensor() || fixture2->IsSensor()) {
			gameObject1.lock()->OnSensorExit(collider2);
			gameObject2.lock()->OnSensorExit(collider1);
		}
		else {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			Vector2 impactVelocity = rigidBody2.lock()->GetVelocity();
			impactVelocity -= rigidBody1.lock()->GetVelocity();
			Vector2 normal = Vector2::Up;
			{
				CollisionData collisionData(gameObject2, collider2, normal, impactVelocity);
				gameObject1.lock()->OnCollisionExit(collisionData);
			}
			{
				CollisionData collisionData(gameObject1, collider1, -normal, -impactVelocity);
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
		ComponentData *data = (ComponentData*)message.data;
		if(data->type == ComponentType::COMPONENT_BOX_COLLIDER_2D) {
			std::shared_ptr<BoxCollider> b = std::static_pointer_cast<BoxCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = b->GetComponent<RigidBody2D>().lock();
			b->fixture = r->body->CreateFixture(b->fixtureDef);
			r->body->ResetMassData();
		}
		else if(data->type == ComponentType::COMPONENT_CIRCLE_COLLIDER) {
			std::shared_ptr<CircleCollider> c = std::static_pointer_cast<CircleCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = c->GetComponent<RigidBody2D>().lock();
			c->fixture = r->body->CreateFixture(c->fixtureDef);
			r->body->ResetMassData();
		}
		else if(data->type == ComponentType::COMPONENT_POLYGON_COLLIDER_2D) {
			std::shared_ptr<PolygonCollider> p = std::static_pointer_cast<PolygonCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = p->GetComponent<RigidBody2D>().lock();
			p->fixture = r->body->CreateFixture(p->fixtureDef);
			r->body->ResetMassData();
		}
		break;
	}
	case MessageType::MESSAGE_TYPE_UNREGISTER_COLLIDER:
	{
		ComponentData *data = (ComponentData*)message.data;
		if(data->type == ComponentType::COMPONENT_BOX_COLLIDER_2D) {
			std::shared_ptr<BoxCollider> b = std::static_pointer_cast<BoxCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = b->GetComponent<RigidBody2D>().lock();
			r->body->DestroyFixture(b->fixture);
			r->body->ResetMassData();
		}
		else if(data->type == ComponentType::COMPONENT_CIRCLE_COLLIDER) {
			std::shared_ptr<CircleCollider> c = std::static_pointer_cast<CircleCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = c->GetComponent<RigidBody2D>().lock();
			r->body->DestroyFixture(c->fixture);
			r->body->ResetMassData();
		}
		else if(data->type == ComponentType::COMPONENT_POLYGON_COLLIDER_2D) {
			std::shared_ptr<PolygonCollider> p = std::static_pointer_cast<PolygonCollider>(data->comp.lock());
			std::shared_ptr<RigidBody2D> r = p->GetComponent<RigidBody2D>().lock();
			r->body->DestroyFixture(p->fixture);
			r->body->ResetMassData();
		}
		break;
	}
	case MessageType::MESSAGE_TYPE_REGISTER_JOINT:
	{
		ComponentData *data = (ComponentData*)message.data;
		if(data->type == ComponentType::COMPONENT_WHEEL_JOINT) {
			std::shared_ptr<WheelJoint> wj = std::static_pointer_cast<WheelJoint>(data->comp.lock());
			wj->joint = world->CreateJoint(wj->jointDef);
		}
		if(data->type == ComponentType::COMPONENT_REVOLUTE_JOINT) {
			std::shared_ptr<RevoluteJoint> rj = std::static_pointer_cast<RevoluteJoint>(data->comp.lock());
			rj->joint = world->CreateJoint(rj->jointDef);
		}
		break;
	}
	case MessageType::MESSAGE_TYPE_UNREGISTER_JOINT:
	{
		ComponentData *data = (ComponentData*)message.data;
		if(data->type == ComponentType::COMPONENT_WHEEL_JOINT) {
			std::shared_ptr<WheelJoint> wj = std::static_pointer_cast<WheelJoint>(data->comp.lock());
			world->DestroyJoint(wj->joint);
		}
		if(data->type == ComponentType::COMPONENT_REVOLUTE_JOINT) {
			std::shared_ptr<RevoluteJoint> rj = std::static_pointer_cast<RevoluteJoint>(data->comp.lock());
			world->DestroyJoint(rj->joint);
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
