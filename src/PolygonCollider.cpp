#include "PolygonCollider.h"
#include "TypeConversion.h"
#include "GameObject.h"
#include "ComponentData.h"
namespace WeiSungEngine {
	PolygonCollider::PolygonCollider() : Collider() {
	}

	PolygonCollider::PolygonCollider(std::weak_ptr<GameObject> gameObject) : Collider(gameObject) {
	}

	PolygonCollider::~PolygonCollider() {

	}

	void PolygonCollider::Init(const Vector2 & offset, std::initializer_list<Vector2> verts, const bool & isSensor, const float & density, const float & friction, const float & restitution) {
		size_t noOfVerts = verts.size();
		assert(noOfVerts >= 3);
		shape = new b2PolygonShape();
		b2Vec2 cVert = TypeConversion::ConvertToB2Vector2(offset);
		b2Vec2 * vertices = new b2Vec2[noOfVerts];
		size_t index = 0;
		Vector2 scale = GetComponent<Transform2D>().lock()->GetScale();
		for (std::initializer_list<Vector2>::iterator it = verts.begin(); it != verts.end(); ++it, ++index) {
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
		this->isSensor = isSensor;
		this->friction = friction;
		this->restitution = restitution;
		this->density = density;
		fixtureDef->shape = shape;
		componentData = new ComponentData();
		componentData->comp = GetWeak();
		componentData->type = Type();
		fixtureDef->userData = componentData;
		std::shared_ptr<GameObject> g = gameObject.lock();
		Message m = Message(MessageScope::MESSAGE_SCOPE_PHYSICS_SYSTEM, MessageType::MESSAGE_TYPE_REGISTER_COLLIDER, componentData);
		g->HandleMessage(m);
		Collider::UpdateCollisionFilter();
		SetEnabled(true);
	}

	Vector2 PolygonCollider::GetOffset() {
		return offset;
	}

	void PolygonCollider::SetOffset(const Vector2 & newOffset) {
		Vector2 difference = newOffset - offset;
		offset = newOffset;
		b2Vec2 newPos;
		for (int i = 0; i < shape->GetVertexCount(); i++) {
			newPos = shape->m_vertices[i] + TypeConversion::ConvertToB2Vector2(difference);
			shape->m_vertices[i].Set(newPos.x, newPos.y);
		}
		newPos = TypeConversion::ConvertToB2Vector2(newOffset);
		shape->m_centroid.Set(newPos.x, newPos.y);
	}
}
