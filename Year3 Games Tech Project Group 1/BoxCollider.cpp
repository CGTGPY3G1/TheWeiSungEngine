#include "BoxCollider.h"
#include "GameObject.h"
BoxCollider::BoxCollider() : Collider() {
}

BoxCollider::BoxCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

BoxCollider::~BoxCollider() {

}

void BoxCollider::Init(const Vector2 & centre, const Vector2 & size, const bool & isSensor, const float & friction, const float & restitution) {
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	fixtureDef->density = 1.f;
	shape = new b2PolygonShape();
	shape->SetAsBox((size.x / 2) * Physics::METRES_PER_PIXEL, (size.y / 2) * Physics::METRES_PER_PIXEL);
	
	SetOffset(centre);
	fixtureDef->shape = shape;
	colliderData = new ColliderData();
	colliderData->comp = GetWeak();
	colliderData->type = Type();
	fixtureDef->userData = colliderData;
	Message m = Message(MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, MessageDataType::MESSAGE_COLLIDER_DATA_TYPE, colliderData);
	gameObject.lock()->HandleMessage(m);
}

void BoxCollider::SetOffset(const Vector2 & newOffset) {
	Vector2 difference = newOffset - offset;
	offset = newOffset;
	b2Vec2 newPos;
	for(int i = 0; i < shape->GetVertexCount(); i++) {
		newPos = shape->m_vertices[i] + TypeConversion::ConvertToB2Vector2(difference);
		shape->m_vertices[i].Set(newPos.x, newPos.y);
	}
	newPos = TypeConversion::ConvertToB2Vector2(newOffset);
	shape->m_centroid.Set(newPos.x, newPos.y);
}

void BoxCollider::SetRotation(const float & angle) {
}

Vector2 BoxCollider::GetOffset() {
	return Vector2();
}
