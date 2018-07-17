#pragma once

#pragma once
#ifndef WS_BLOOD_SPLATTER_SCRIPT_H
#define WS_BLOOD_SPLATTER_SCRIPT_H
#include "ScriptableComponent.h"

class BloodSplatterScript : public ScriptableComponent {
public:
	BloodSplatterScript();
	BloodSplatterScript(std::weak_ptr<GameObject> gameObject);
	~BloodSplatterScript();
	const ComponentType Type() const override { return COMPONENT_BLOOD_SPLATTER_SCRIPT; }
	void Update(const float & deltaTime) override;
	const std::string GetName() const override { return "BloodSplatterScript"; }
	int GetSortOrder() override;
	void Bleed();
	void Pool();
	const int GetBleedSeverity() const;
	const bool IsBleeding() const;
	void SetBleeding(const int & severity);
private:
	int severity = 0;
	const float MIN_TIME = 3.0F, MAX_TIME = 10.0f;
	float bleedTime = 10.0f;
};

#endif // !WS_BLOOD_SPLATTER_SCRIPT_H