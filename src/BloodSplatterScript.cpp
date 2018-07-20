#include "BloodSplatterScript.h"
#include "TypeInfo.h"
#include "GameObjectFactory.h"
namespace WeiSungEngine {
	BloodSplatterScript::BloodSplatterScript() : ScriptableComponent() {
	}

	BloodSplatterScript::BloodSplatterScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
	}

	BloodSplatterScript::~BloodSplatterScript() {
	}

	void BloodSplatterScript::Update(const float & deltaTime) {

		if (severity > 0) {
			bleedTime -= deltaTime;

			if (bleedTime <= 0.0f) {
				Bleed();
			}
		}
	}

	int BloodSplatterScript::GetSortOrder() {
		return TypeInfo::ScriptSortOrder<BloodSplatterScript>();
	}

	void BloodSplatterScript::Bleed() {
		std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
		if (t) {
			const int noOfSplats = Random::RandomInt(1, severity + 1);
			for (int i = 0; i < noOfSplats; i++) {
				GameObjectFactory::CreateBloodSplat(t->GetPosition() + Vector2(Random::RandomFloat(-32.0f, 32.0f), Random::RandomFloat(-32.0f, 32.0f)), 1);
			}
			bleedTime = Random::RandomFloat(MIN_TIME / severity, MAX_TIME / severity);
		}
	}

	void BloodSplatterScript::Pool() {
		std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
		if (t) {
			GameObjectFactory::CreateBloodSplat(t->GetPosition(), 0);
		}
	}

	const int BloodSplatterScript::GetBleedSeverity() const {
		return severity;
	}

	const bool BloodSplatterScript::IsBleeding() const {
		return severity > 0;
	}

	void BloodSplatterScript::SetBleeding(const int & severity) {
		this->severity = severity;
		if (severity > 0) Bleed();
	}
}
