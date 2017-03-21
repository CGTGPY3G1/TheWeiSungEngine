#pragma once
#ifndef WS_HEALTH_SCRIPT_H
#define WS_HEALTH_SCRIPT_H
#include "ScriptableComponent.h"

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
	const float GetHealthAsPercentage() const;
	void SetHealth(const float newHealth);
	void AddToHealth(const float amount);
	void Hit(const float force);
	void Reset();

private:
	float health = 50, maxHealth = 50;
	bool alive = true;
};

#endif // !WS_HEALTH_SCRIPT_H