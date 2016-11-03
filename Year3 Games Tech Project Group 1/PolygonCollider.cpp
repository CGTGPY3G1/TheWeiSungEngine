#include "PolygonCollider.h"
#include "TypeConversion.h"
#include "GameObject.h"
PolygonCollider::PolygonCollider() : Collider() {
}

PolygonCollider::PolygonCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
}

PolygonCollider::~PolygonCollider() {

}

void PolygonCollider::Init(const Vector2 & center, std::initializer_list<Vector2> verts, const bool & isSensor, const float & friction, const float & restitution) {
	unsigned int noOfVerts = verts.size();
	assert(noOfVerts >= 3);
	shape = new b2PolygonShape();
	b2Vec2 cVert = TypeConversion::ConvertToB2Vector2(center);
	shape->m_centroid.Set(cVert.x, cVert.y);
	b2Vec2 * vertices = new b2Vec2[noOfVerts];
	size_t index = 0;
	//vertices[index].Set(cVert.x, cVert.y);
	//index++;
	for(std::initializer_list<Vector2>::iterator it = verts.begin(); it != verts.end(); ++it, ++index) {
		cVert = TypeConversion::ConvertToB2Vector2(center + (*it));
		vertices[index].Set(cVert.x, cVert.y);
	}
	shape->Set(vertices, noOfVerts);
	fixtureDef = new b2FixtureDef();
	fixtureDef->isSensor = isSensor;
	fixtureDef->friction = friction;
	fixtureDef->restitution = restitution;
	fixtureDef->density = 1.0f;
	fixtureDef->shape = shape;
	colliderData = new ColliderData();
	colliderData->comp = GetWeak();
	colliderData->type = Type();
	fixtureDef->userData = colliderData;
	Message m = Message(MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, MessageDataType::MESSAGE_COLLIDER_DATA_TYPE, colliderData);
	gameObject.lock()->HandleMessage(m);
}
