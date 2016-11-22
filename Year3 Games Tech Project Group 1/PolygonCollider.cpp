#include "PolygonCollider.h"
#include "TypeConversion.h"
#include "GameObject.h"
PolygonCollider::PolygonCollider() : Collider() {
}

PolygonCollider::PolygonCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

PolygonCollider::~PolygonCollider() {

}

void PolygonCollider::Init(const Vector2 & offset, std::initializer_list<Vector2> verts, const bool & isSensor, const float & density, const float & friction, const float & restitution) {
	unsigned int noOfVerts = verts.size();
	assert(noOfVerts >= 3);
	shape = new b2PolygonShape();
	b2Vec2 cVert = TypeConversion::ConvertToB2Vector2(offset);
	b2Vec2 * vertices = new b2Vec2[noOfVerts];
	size_t index = 0;
	Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
	for(std::initializer_list<Vector2>::iterator it = verts.begin(); it != verts.end(); ++it, ++index) {
		cVert = TypeConversion::ConvertToB2Vector2((*it));
		vertices[index].Set(cVert.x * scale.x, cVert.y * scale.y);
	}
	shape->Set(vertices, noOfVerts);
	SetOffset(offset);
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	fixtureDef->density = density;
	fixtureDef->shape = shape;
	colliderData = new ColliderData();
	colliderData->comp = GetWeak();
	colliderData->type = Type();
	fixtureDef->userData = colliderData;
	Message m = Message(MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, MessageDataType::MESSAGE_COLLIDER_DATA_TYPE, colliderData);
	gameObject.lock()->HandleMessage(m);
	SetEnabled(true);
}

Vector2 PolygonCollider::GetOffset() {
	return offset;
}

void PolygonCollider::SetOffset(const Vector2 & newOffset) {
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
