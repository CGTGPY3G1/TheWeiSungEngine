#include "Collider.h"
#include "GameObject.h"
#include "RigidBody2D.h"

Collider::Collider() : Component(){
}

Collider::Collider(std::weak_ptr<GameObject> gameObject) : Component(gameObject) {
	std::shared_ptr<GameObject> g = gameObject.lock();
	if(!g->HasComponent<RigidBody2D>()) {
		std::shared_ptr<RigidBody2D> r = g->AddComponent<RigidBody2D>().lock();
		r->Init();
	}
}

Collider::~Collider() {
	if(colliderData) delete colliderData;
	colliderData = NULL;
}

bool Collider::IsSensor() {
	return fixture->IsSensor();
}

void Collider::SetSensor(const bool & sensor) {
	fixture->SetSensor(sensor);
}
