#include "CircleCollider.h"
#include "GameObject.h"
#include "ComponentData.h"

CircleCollider::CircleCollider() : Collider() {
}

CircleCollider::CircleCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

CircleCollider::~CircleCollider() {

}

void CircleCollider::Init(const Vector2 & position, const float & radius, const bool & isSensor, const float & density, const float & friction, const float & restitution) {
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	fixtureDef->density = density;
	shape = new b2CircleShape();
	std::shared_ptr<RigidBody2D> r = GetComponent<RigidBody2D>().lock();
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	shape->m_radius = radius * Physics::METRES_PER_PIXEL * std::max<float>(scale.x, scale.y);
	SetOffset(position);
	fixtureDef->shape = shape;
	componentData = new ComponentData();
	componentData->comp = GetWeak();
	componentData->type = Type();
	fixtureDef->userData = componentData;
	Message m = Message(MessageScope::MESSAGE_SCOPE_PHYSICS_SYSTEM, MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, componentData);
	gameObject.lock()->HandleMessage(m);
	SetEnabled(true);
	Collider::UpdateCollisionFilter();
}

Vector2 CircleCollider::GetOffset() {
	return TypeConversion::ConvertToVector2(shape->m_p);
}

void CircleCollider::SetOffset(const Vector2 & newOffset) {
	shape->m_p = TypeConversion::ConvertToB2Vector2(newOffset);
}
