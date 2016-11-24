#include "BoxCollider.h"
#include "GameObject.h"
#include "ComponentData.h"

BoxCollider::BoxCollider() : Collider() {
}

BoxCollider::BoxCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

BoxCollider::~BoxCollider() {

}

void BoxCollider::Init(const Vector2 & offset, const Vector2 & size, const bool & isSensor, const float & density, const float & friction, const float & restitution) {
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	fixtureDef->density = density;
	shape = new b2PolygonShape();
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	shape->SetAsBox((size.x / 2) * Physics::METRES_PER_PIXEL * scale.x, (size.y / 2) * Physics::METRES_PER_PIXEL * scale.y);
	SetOffset(offset);
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
