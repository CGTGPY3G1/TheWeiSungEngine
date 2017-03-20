#include "PhysicsSystem.h"
#include "Physics.h"
#include "Engine.h"
#include "DebugDraw.h"
#include "Math.h"

#include "GameObject.h"
#include "ComponentData.h"
#include "CollisionData.h"
float32 PhysicsSystem::ReportFixture(b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction) {
	if(!reportSensors && fixture->IsSensor()) return -1;
	const int oCat = fixture->GetFilterData().categoryBits;
	if((oCat & raycastFilter) == oCat) {
		ComponentData * colliderData = (ComponentData *)fixture->GetUserData();
		if(colliderData) {
			if(!hit.hit) {
				hit.hit = true;
			}
			if(fraction < hit.fraction) {
				hit.collider = std::static_pointer_cast<Collider>(colliderData->comp.lock());
				hit.normal = Vector2(normal.x, normal.y);
				hit.point = TypeConversion::ConvertToVector2(point);
				hit.fraction = (float)fraction;
			}
		}
	}
	return 1;
}

bool PhysicsSystem::ReportFixture(b2Fixture* fixture) {
	if(!reportSensors && fixture->IsSensor()) return true;
	const int oCat = fixture->GetFilterData().categoryBits;
	if((oCat & aabbFilter) == oCat) {
		if(checkMultipleAABBs) {
			ComponentData * colliderData = (ComponentData *)fixture->GetUserData();
			if(colliderData) {
				aabbHits.push_back(std::static_pointer_cast<Collider>(colliderData->comp.lock()));
			}
		}
		else {
			aabbHit = true;
			return false;
		}
	}
	return true;
}

PhysicsSystem::PhysicsSystem(){
	world = new b2World(b2Vec2(0, 0));
	world->SetAllowSleeping(true);
	
	DebugDraw * debugDraw = Engine::GetInstance().GetDebugDraw();
	debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
	world->SetDebugDraw(debugDraw);
	world->SetContactListener(this);
}

PhysicsSystem & PhysicsSystem::GetInstance() {
	static PhysicsSystem instance = PhysicsSystem();
	return instance;
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(const float & deltaTime) {
	collidingID1 = 0; collidingID2 = 0;
	world->Step(deltaTime, settings.velocityIterations, settings.positionIterations);
}

void PhysicsSystem::UpdateBodies() {
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if(b->GetType() == b2BodyType::b2_dynamicBody) {
			RigidBodyData * rb = (RigidBodyData *)b->GetUserData();
			if(rb) {
				std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(rb->data.lock());
				if(r) {
					std::shared_ptr<Transform2D> t = r->GetComponent<Transform2D>().lock();
					if(t) {
						t->SetPosition(TypeConversion::ConvertToVector2(b->GetPosition()));
						t->SetRotation(b->GetAngle() * Math::RadiansToDegrees());
					}
				}
			}
		}
		else if(b->GetType() == b2BodyType::b2_kinematicBody || b->GetType() == b2BodyType::b2_staticBody) {
			RigidBodyData * rb = (RigidBodyData *)b->GetUserData();
			if(rb) {
				std::shared_ptr<RigidBody2D> r = std::static_pointer_cast<RigidBody2D>(rb->data.lock());
				if(r) {
					std::shared_ptr<Transform2D> t = r->GetComponent<Transform2D>().lock();
					if(t) {
						b2Vec2 newPos = TypeConversion::ConvertToB2Vector2(t->GetPosition());
						float newAngle = t->GetRotation() * Math::DegreesToRadians();
						b->SetTransform(newPos, newAngle);
					}
				}
			}
		}
	}
	//if(dirty) dirty = false;
}

void PhysicsSystem::SetGravity(const float & x, const float & y) {
	world->SetGravity(b2Vec2(x, y));
}

void PhysicsSystem::BeginContact(b2Contact* contact) {
	b2Fixture * fixture1 = contact->GetFixtureA(), *fixture2 = contact->GetFixtureB();	
	b2Body * body1 = fixture1->GetBody(), *body2 = fixture2->GetBody();
	if(body1 && body2) {
		ComponentData * colliderData1 = (ComponentData *)fixture1->GetUserData();
		ComponentData * colliderData2 = (ComponentData *)fixture2->GetUserData();
		if(colliderData1 && colliderData2) {
			std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
			std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
			RigidBodyData * rigidBodyData1 = (RigidBodyData *)body1->GetUserData();
			RigidBodyData * rigidBodyData2 = (RigidBodyData *)body2->GetUserData();
			std::shared_ptr<RigidBody2D> rigidBody1 = std::static_pointer_cast<RigidBody2D>(rigidBodyData1->data.lock());
			std::shared_ptr<RigidBody2D> rigidBody2 = std::static_pointer_cast<RigidBody2D>(rigidBodyData2->data.lock());
			if(rigidBody1 && rigidBody2) {
				std::shared_ptr<GameObject> gameObject1 = rigidBody1->GetGameObject().lock(), gameObject2 = rigidBody2->GetGameObject().lock();
				if(gameObject1->combinedObjectID == gameObject2->combinedObjectID) return;
				if(fixture2->IsSensor() || fixture1->IsSensor()) {
					gameObject1->OnSensorEnter(collider2);
					gameObject2->OnSensorEnter(collider1);
				}
				else {
					b2WorldManifold worldManifold;
					contact->GetWorldManifold(&worldManifold);
					Vector2 impactVelocity = rigidBody2->GetVelocity() - rigidBody1->GetVelocity();
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
						gameObject1->OnCollisionEnter(collisionData);
					}
					{
						CollisionData collisionData = CollisionData(gameObject1, collider1, -normal, -impactVelocity);
						for(size_t i = 0; i < cps; i++) {
							collisionData.contactPoints[i] = contactPoints[i];
						}
						gameObject2->OnCollisionEnter(collisionData);
					}
				}
			}
		}
	}
}

void PhysicsSystem::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse) {
	b2Fixture * fixture1 = contact->GetFixtureA(), *fixture2 = contact->GetFixtureB();
	b2Body * body1 = fixture1->GetBody(), *body2 = fixture2->GetBody();
	if(body1 && body2) {
		ComponentData * colliderData1 = (ComponentData *)fixture1->GetUserData();
		ComponentData * colliderData2 = (ComponentData *)fixture2->GetUserData();
		if(colliderData1 && colliderData2) {
			std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
			std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
			RigidBodyData * rigidBodyData1 = (RigidBodyData *)body1->GetUserData();
			RigidBodyData * rigidBodyData2 = (RigidBodyData *)body2->GetUserData();
			std::shared_ptr<RigidBody2D> rigidBody1 = std::static_pointer_cast<RigidBody2D>(rigidBodyData1->data.lock());
			std::shared_ptr<RigidBody2D> rigidBody2 = std::static_pointer_cast<RigidBody2D>(rigidBodyData2->data.lock());
			if(rigidBody1 && rigidBody2) {
				std::shared_ptr<GameObject> gameObject1 = rigidBody1->GetGameObject().lock();
				std::shared_ptr<GameObject> gameObject2 = rigidBody2->GetGameObject().lock();
				if(gameObject1->combinedObjectID == gameObject2->combinedObjectID) return;

				b2WorldManifold worldManifold;
				contact->GetWorldManifold(&worldManifold);
				Vector2 impactVelocity = rigidBody2->GetVelocity() - rigidBody1->GetVelocity();
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
					gameObject1->OnCollisionStay(collisionData);
				}
				{
					CollisionData collisionData = CollisionData(gameObject1, collider1, -normal, -impactVelocity);
					for(size_t i = 0; i < cps; i++) {
						collisionData.contactPoints[i] = contactPoints[i];
					}
					gameObject2->OnCollisionStay(collisionData);
				}
			}		
		}
	}

}

void PhysicsSystem::EndContact(b2Contact* contact) {
	b2Fixture * fixture1 = contact->GetFixtureA(), *fixture2 = contact->GetFixtureB();
	b2Body * body1 = fixture1->GetBody(), *body2 = fixture2->GetBody();
	if(body1 && body2) {
		ComponentData * colliderData1 = (ComponentData *)fixture1->GetUserData();
		ComponentData * colliderData2 = (ComponentData *)fixture2->GetUserData();
		if(colliderData1 && colliderData2) {
			std::weak_ptr<Collider> collider1 = std::static_pointer_cast<Collider>(colliderData1->comp.lock());
			std::weak_ptr<Collider> collider2 = std::static_pointer_cast<Collider>(colliderData2->comp.lock());
			RigidBodyData * rigidBodyData1 = (RigidBodyData *)body1->GetUserData();
			RigidBodyData * rigidBodyData2 = (RigidBodyData *)body2->GetUserData();
			std::shared_ptr<RigidBody2D> rigidBody1 = std::static_pointer_cast<RigidBody2D>(rigidBodyData1->data.lock());
			std::shared_ptr<RigidBody2D> rigidBody2 = std::static_pointer_cast<RigidBody2D>(rigidBodyData2->data.lock());
			if(rigidBody1 && rigidBody2) {
				std::weak_ptr<GameObject> g1 = rigidBody1->GetGameObject();
				std::weak_ptr<GameObject> g2 = rigidBody2->GetGameObject();
				if(!g1.expired() && !g2.expired()) {
					std::shared_ptr<GameObject> gameObject1 = rigidBody1->GetGameObject().lock();
					std::shared_ptr<GameObject> gameObject2 = rigidBody2->GetGameObject().lock();
					if(gameObject1->combinedObjectID == gameObject2->combinedObjectID) return;
					if(fixture1->IsSensor() || fixture2->IsSensor()) {
						gameObject1->OnSensorExit(collider2);
						gameObject2->OnSensorExit(collider1);
					}
					else {
						b2WorldManifold worldManifold;
						contact->GetWorldManifold(&worldManifold);
						Vector2 impactVelocity = rigidBody2->GetVelocity() - rigidBody1->GetVelocity();
						Vector2 normal = Vector2::Up;
						{
							CollisionData collisionData(gameObject2, collider2, normal, impactVelocity);
							gameObject1->OnCollisionExit(collisionData);
						}
						{
							CollisionData collisionData(gameObject1, collider1, -normal, -impactVelocity);
							gameObject2->OnCollisionExit(collisionData);
						}
					}
				}
			}
		}
	}
}

RayCastHit PhysicsSystem::RayCast(const Vector2 & start, const Vector2 & end, const bool & reportSensors, const int & collisionMask) {
	this->reportSensors = reportSensors;
	raycastFilter = collisionMask;
	hit.Reset();
	world->RayCast(this, TypeConversion::ConvertToB2Vector2(start), TypeConversion::ConvertToB2Vector2(end));
	if(!this->reportSensors) this->reportSensors = true;
	return hit;
}

std::vector<std::weak_ptr<Collider>> PhysicsSystem::CircleCast(const Vector2 & position, const float & radius, const bool & reportSensors, const int & collisionMask) {
	this->reportSensors = reportSensors;
	checkMultipleAABBs = true;
	aabbFilter = collisionMask;
	aabbHits.clear();
	world->QueryAABB(this, TypeConversion::ConvertToB2AABB(AABB(position.y + radius, position.x - radius, position.y - radius, position.x + radius)));
	std::vector<std::weak_ptr<Collider>>::iterator it = aabbHits.begin();
	const float squareRadius = radius * radius;
	while(it != aabbHits.end()) {
		if(((*it).lock()->GetComponent<Transform2D>().lock()->GetPosition() - position).SquareMagnitude() > squareRadius) it = aabbHits.erase(it);
		else ++it;
	}
	if(!this->reportSensors) this->reportSensors = true;
	return aabbHits;
}

bool PhysicsSystem::CheckAABB(const AABB & aabb, const bool & reportSensors, const int & collisionMask) {
	this->reportSensors = reportSensors;
	checkMultipleAABBs = false;
	aabbFilter = collisionMask;
	world->QueryAABB(this, TypeConversion::ConvertToB2AABB(aabb));
	if(aabbHit) {
		aabbHit = false;
		return true;
	}
	return aabbHit;
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
		b2Body *data = (b2Body*)message.data;
		world->DestroyBody(data);
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
		break;
	}
	case MessageType::MESSAGE_TYPE_UNREGISTER_JOINT:
	{
		ComponentData *data = (ComponentData*)message.data;
		if(data->type == ComponentType::COMPONENT_WHEEL_JOINT) {
			std::shared_ptr<WheelJoint> wj = std::static_pointer_cast<WheelJoint>(data->comp.lock());
			world->DestroyJoint(wj->joint);
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
