#include "CollisionData.h"
#include "GameObject.h"
CollisionData::CollisionData(const std::weak_ptr<GameObject>& otherGameObject, const std::weak_ptr<Collider>& otherFixture, const Vector2 & cNormal, const Vector2 & velocity) :
	gameObject(otherGameObject), fixture(otherFixture), normal(cNormal), relativeVelocity(velocity) {
}