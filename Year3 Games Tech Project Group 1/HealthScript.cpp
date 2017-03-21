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
			std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
			if(bs) bs->Pool();
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

const float HealthScript::GetHealthAsPercentage() const {
	if(maxHealth <= 0.0f) return 0.0f;
	return (health / maxHealth) * 100.0f;
}

void HealthScript::SetHealth(const float newHealth) {
	health = newHealth;
	maxHealth = newHealth;
}

void HealthScript::AddToHealth(const float amount) {
	health += amount;
	if(health > maxHealth) health = maxHealth;
}

void HealthScript::Hit(const float force) {
	health -= force;
	if(health > 0.0f) {
		std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
		if(bs) {
			const float rHealth = health / maxHealth;
			if(rHealth < 0.8f)
				bs->SetBleeding(rHealth > 0.5f ? 1 : (rHealth > 0.25f ? 2 : 3));
		}
	}
}

void HealthScript::Reset() {
	health = maxHealth;
	std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
	if(bs) {
		bs->SetBleeding(0);
	}
}
