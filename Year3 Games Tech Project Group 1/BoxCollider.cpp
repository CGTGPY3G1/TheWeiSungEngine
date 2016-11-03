#include "BoxCollider.h"
#include "GameObject.h"
BoxCollider::BoxCollider() : Collider() {
}

BoxCollider::BoxCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

BoxCollider::~BoxCollider() {

}

void BoxCollider::Init(const Vector2 & position, const Vector2 & size, const bool & isSensor, const float & friction, const float & restitution) {
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	fixtureDef->density = 1.f;
	shape = new b2PolygonShape();
	shape->SetAsBox((size.x / 2) * Physics::METRES_PER_PIXEL, (size.y / 2) * Physics::METRES_PER_PIXEL);
	
	SetPosition(position);
	fixtureDef->shape = shape;
	colliderData = new ColliderData();
	colliderData->comp = GetWeak();
	colliderData->type = Type();
	fixtureDef->userData = colliderData;
	Message m = Message(MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, MessageDataType::MESSAGE_COLLIDER_DATA_TYPE, colliderData);
	gameObject.lock()->HandleMessage(m);
}

void BoxCollider::SetPosition(const Vector2 & newPosition) {
	for(size_t i = 0; i < shape->GetVertexCount(); i++) {
		b2Vec2 newPos = shape->m_vertices[i];
		newPos += TypeConversion::ConvertToB2Vector2(newPosition);
		shape->m_vertices[i].Set(newPos.x, newPos.y);
	}
	b2Vec2 newPos = TypeConversion::ConvertToB2Vector2(newPosition);
	shape->m_centroid.Set(newPos.x, newPos.y);
}

void BoxCollider::SetRotation(const float & angle) {
}
