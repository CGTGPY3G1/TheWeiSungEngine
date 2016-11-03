#include "CircleCollider.h"
#include "GameObject.h"
CircleCollider::CircleCollider() : Collider() {
}

CircleCollider::CircleCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

CircleCollider::~CircleCollider() {

}

void CircleCollider::Init(const Vector2 & position, const float & radius, const bool & isSensor, const float & friction, const float & restitution) {
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	shape = new b2CircleShape();
	shape->m_p.Set(position.x * Physics::METRES_PER_PIXEL, position.y * Physics::METRES_PER_PIXEL);
	shape->m_radius = radius* Physics::METRES_PER_PIXEL;
	fixtureDef->shape = shape;
	colliderData = new ColliderData();
	colliderData->comp = GetWeak();
	colliderData->type = Type();
	fixtureDef->userData = colliderData;
	Message m = Message(MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, MessageDataType::MESSAGE_COLLIDER_DATA_TYPE, colliderData);
	gameObject.lock()->HandleMessage(m);
}
