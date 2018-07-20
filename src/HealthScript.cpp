#include "HealthScript.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "PopulationController.h"

namespace WeiSungEngine {
	AttackerInfo lastAttacker;
	HealthScript::HealthScript() : ScriptableComponent() {
	}

	HealthScript::HealthScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
	}

	HealthScript::~HealthScript() {
	}

	void HealthScript::Start() {
	}

	void HealthScript::Update(const float & deltaTime) {
		if (alive) {
			if (health <= 0.0f) {
				alive = false;
				std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
				if (bs) bs->Pool();
				std::shared_ptr<GameObject> g = gameObject.lock();
				if (g) g->Destroy();
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
		if (maxHealth <= 0.0f) return 0.0f;
		return (health / maxHealth) * 100.0f;
	}

	void HealthScript::SetHealth(const float & newHealth) {
		health = newHealth;
		maxHealth = newHealth;
	}

	void HealthScript::AddToHealth(const float & amount) {
		health += amount;
		if (health > maxHealth) health = maxHealth;
		else if (health < 0) health = 0;
	}

	bool HealthScript::Hit(const float & damage) {
		if (health > 0.0f) {
			health -= damage;
			std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
			if (bs) {
				const float relativeHealth = health / maxHealth;
				if (relativeHealth < 0.8f) {
					bs->SetBleeding(relativeHealth > 0.5f ? 1 : (relativeHealth > 0.25f ? 2 : 3));
				}
				if (health <= 0.0f) {
					std::shared_ptr<CharacterScript> cs = GetComponent<CharacterScript>().lock();
					if (cs) {
						std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
						GameObjectFactory::ScheduleCollectableCreation("Collectable", CollectableHealth, 20, t->GetPosition(), Vector2(2.0f, 2.0f));
						PopulationController::GetInstance().RegisterKill(lastAttacker, cs->GetCharacterID(), cs->GetCharacterName());
					}
					return true;
				}
			}
		}
		return false;
	}

	bool HealthScript::Hit(const float & damage, const AttackerInfo & attacker) {
		if (health > 0.0f) {
			lastAttacker = attacker;
			health -= damage;
			std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
			if (bs) {
				const float relativeHealth = health / maxHealth;
				if (relativeHealth < 0.8f) {
					bs->SetBleeding(relativeHealth > 0.5f ? 1 : (relativeHealth > 0.25f ? 2 : 3));
				}
				if (health <= 0.0f) {
					std::shared_ptr<CharacterScript> cs = GetComponent<CharacterScript>().lock();
					if (cs) {
						std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
						GameObjectFactory::ScheduleCollectableCreation("Collectable", CollectableHealth, 20, t->GetPosition(), Vector2(2.0f, 2.0f));
						PopulationController::GetInstance().RegisterKill(attacker, cs->GetCharacterID(), cs->GetCharacterName());
					}
					return true;
				}
			}
		}
		return false;
	}

	void HealthScript::ExplosionHit(const Vector2 & force, const float & damage, const float & distanceFactor, const AttackerInfo & attacker) {
		if (health > 0.0f) {
			if (Hit(damage, attacker)) {
				static const float minMagnitude = 2.0f * 2.0f;
				std::shared_ptr<Transform2D> transform = GetComponent<Transform2D>().lock();
				if (force.SquareMagnitude() > minMagnitude) {
					GameObjectFactory::CreateBloodSpray(transform->GetPosition() + force * 0.5f, force, Vector2(distanceFactor, distanceFactor));
				}
			}
		}
	}

	void HealthScript::Reset() {
		health = maxHealth;
		std::shared_ptr<BloodSplatterScript> bs = GetComponent<BloodSplatterScript>().lock();
		if (bs) {
			bs->SetBleeding(0);
		}
	}
}

