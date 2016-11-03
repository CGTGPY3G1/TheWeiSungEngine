#include "PhysicsSystem.h"
#include "Physics.h"
#include "Engine.h"
#include "DebugDraw.h"
#include "Scene.h"
#include "TypeInfo.h"
#include "Math.h"
PhysicsSystem::PhysicsSystem(Scene * owner) : scene(owner) {
	world = new b2World(b2Vec2(0, 0));
	world->SetAllowSleeping(false);
	world->SetDebugDraw(Engine::GetInstance().GetDebugDraw());
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
