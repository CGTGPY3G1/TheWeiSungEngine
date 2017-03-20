#include "DamageScript.h"
#include "RigidBody2D.h"
#include "GameObject.h"
#include "HealthScript.h"

DamageScript::DamageScript() {
}

DamageScript::DamageScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

DamageScript::~DamageScript() {
}

void DamageScript::Start() {
	rigidBody = GetComponent<RigidBody2D>();
}

int DamageScript::GetSortOrder() {
	return TypeInfo::ScriptSortOrder<DamageScript>();
}

void DamageScript::OnCollisionEnter(const CollisionData & data) {
	if(rigidBody.use_count() > 0) {		
		std::shared_ptr<GameObject> g = data.gameObject.lock();
		if(g) {
			std::shared_ptr<RigidBody2D> rb = rigidBody.lock();
			std::shared_ptr<RigidBody2D> rb2 = g->GetComponent<RigidBody2D>().lock();
			std::shared_ptr<HealthScript> hs = g->GetComponent<HealthScript>().lock();
			if(rb && rb2 && hs) {
				float damage = ((rb->GetVelocity()) - (rb2->GetVelocity())).Magnitude() * Physics::METRES_PER_PIXEL;
				if(damage > 8.0f) hs->Hit(damage);
			}
		}
	}
	
}
