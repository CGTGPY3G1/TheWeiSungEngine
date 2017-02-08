#pragma once
#ifndef WS_HEALTH_SCRIPT_H
#define WS_HEALTH_SCRIPT_H
#include "ScriptableComponent.h"
#include "cereal\cereal.hpp"
#include "cereal\access.hpp"
#include "cereal\details\traits.hpp"
#include <cereal\types\polymorphic.hpp>

class HealthScript : public ScriptableComponent {
public:
	HealthScript();
	HealthScript(std::weak_ptr<GameObject> gameObject);
	~HealthScript();
	const ComponentType Type() const override { return COMPONENT_HEALTH_SCRIPT; }
	void Start() override;
	void Update(const float & deltaTime) override;
	const std::string GetName() const override { return "HealthScript"; }
	int GetSortOrder() override;

	float GetHealth();
	const float GetHealth() const;
	void SetHealth(const float newHealth);
	void AddToHealth(const float amount);

	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {
		ScriptableComponent::save(ar);
	}
private:
	float health = 50;
	bool alive = true;
};

#endif // !WS_HEALTH_SCRIPT_H