#include "Collider.h"
#include "GameObject.h"
#include "RigidBody2D.h"
#include "ComponentData.h"
namespace WeiSungEngine {
	Collider::Collider() : Component() {
	}

	Collider::Collider(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
		std::shared_ptr<GameObject> g = gameObject.lock();
		if (!g->HasComponent<RigidBody2D>()) {
			std::shared_ptr<RigidBody2D> r = g->AddComponent<RigidBody2D>().lock();
			r->Init();
		}
	}

	Collider::~Collider() {
	}

	bool Collider::IsSensor() {
		return fixture->IsSensor();
	}

	void Collider::SetSensor(const bool & sensor) {
		fixture->SetSensor(sensor);
	}

	void Collider::SetCollisionFilter(const int & collisionCategory, const int & collisionMask) {
		b2Filter filter = fixture->GetFilterData();
		if (collisionCategory == CATEGORY_PLAYER || collisionCategory == CATEGORY_AI_CHARACTER)
			int o = 0;
		filter.categoryBits = collisionCategory;
		filter.maskBits = collisionMask;
		fixtureDef->filter = filter;
		fixture->SetFilterData(filter);
	}

	void Collider::SetCollisionCategory(const int & collisionCategory) {
		b2Filter filter = fixture->GetFilterData();
		filter.categoryBits = collisionCategory;
		fixtureDef->filter = filter;
		fixture->SetFilterData(filter);
	}

	int Collider::GetCollisionCategory() {
		return fixture->GetFilterData().categoryBits;
	}

	void Collider::SetCollisionMask(const int & collisionMask) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = collisionMask;
		fixtureDef->filter = filter;
		fixture->SetFilterData(filter);
	}

	int Collider::GetCollisionMask() {
		return fixture->GetFilterData().maskBits;
	}

	AABB Collider::GetAABB() {
		b2AABB aabb;
		fixture->GetShape()->ComputeAABB(&aabb, GetComponent<RigidBody2D>().lock()->GetBody()->GetTransform(), 0);
		return TypeConversion::ConvertToAABB(aabb);
	}

	void Collider::UpdateCollisionFilter() {
		std::shared_ptr<GameObject> g = gameObject.lock();
		SetCollisionFilter(g->GetCollisionCategory(), g->GetCollisionMask());
	}
}
