#include "HealthScript.h"
#include "GameObject.h"

HealthScript::HealthScript() : ScriptableComponent() {
}

HealthScript::HealthScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

HealthScript::~HealthScript() {
}

void HealthScript::Start() {
}

void HealthScript::Update(const float & deltaTime) {
	if(alive) {
		if(health <= 0.00001f) {
			alive = false;
			std::shared_ptr<GameObject> g = gameObject.lock();
			if(g) g->Destroy();
		}
	}
}

int HealthScript::GetSortOrder() {
	static const unsigned int order = TypeInfo::ScriptSortOrder<HealthScript>();
	return order;
}

float HealthScript::GetHealth() {
	return health;
}

const float HealthScript::GetHealth() const {
	return health;
}

void HealthScript::SetHealth(const float newHealth) {
	health = newHealth;
}

void HealthScript::AddToHealth(const float amount) {
	health += amount;
}
