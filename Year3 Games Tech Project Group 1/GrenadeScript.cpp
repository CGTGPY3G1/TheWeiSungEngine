#include "GrenadeScript.h"
#include "PhysicsSystem.h"
#include "Transform2D.h"
#include "GameObject.h"
#include "HealthScript.h"
GrenadeScript::GrenadeScript() {
}

GrenadeScript::GrenadeScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

GrenadeScript::~GrenadeScript() {
}

void GrenadeScript::Detonate() {
	std::shared_ptr<Transform2D> transform = GetComponent<Transform2D>().lock();
	const Vector2 position = transform->GetPosition();
	std::vector<std::weak_ptr<Collider>> colliders = PhysicsSystem::GetInstance().CircleCast(position, radius);
	for(std::vector<std::weak_ptr<Collider>>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		std::shared_ptr<Collider> c = (*it).lock();
		if(c) {
			if(c->GetGameObjectID() == GetGameObjectID()) continue;
			std::shared_ptr<RigidBody2D> rb = c->GetComponent<RigidBody2D>().lock();
			if(rb) {
				Vector2 cPosition = rb->GetPosition();
				const float distance = (cPosition - position).Magnitude();
				Vector2 direction = (cPosition - position).Normalized();
				const float distanceScale = (radius / (distance != 0 ? distance : radius));
				rb->AddForce(direction * distanceScale * explosionForce, ForceType::IMPULSE_FORCE);
				std::shared_ptr<HealthScript> healthScript = c->GetComponent<HealthScript>().lock();
				if(healthScript) {
					healthScript->Hit(damage * distanceScale);
				}
			}
		}
	}
	gameObject.lock()->Destroy();
	alive = false;
}

void GrenadeScript::Update(const float & deltaTime) {
	if(lifeTime > 0.0f) lifeTime -= deltaTime;
	else if(alive) Detonate();
}

int GrenadeScript::GetSortOrder() {
	return 0;
}

void GrenadeScript::SetTimer(const float & time) {
	lifeTime = time;
}

const float GrenadeScript::GetBlastRadius() const {
	return radius;
}

void GrenadeScript::SetBlastRadius(const float & radius) {
	this->radius = radius;
}

const float GrenadeScript::GetDamage() const {
	return damage;
}

void GrenadeScript::SetDamage(const float & damage) {
	this->damage = damage;
}
