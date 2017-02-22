#pragma once
#ifndef WS_RAMPAGE_SCRIPT_H
#define WS_RAMPAGE_SCRIPT_H

#include "ScriptableComponent.h"
class RigidBody2D;
class TileMapper;
class Transform2D;
class RampageScript : public ScriptableComponent {
public:
	RampageScript();
	RampageScript(std::weak_ptr<GameObject> gameObject);
	~RampageScript();
	void Start() override;
	void Update(const float & deltaTime) override;
	void Render() override;
	const ComponentType Type() const override { return COMPONENT_RAMPAGE_SCRIPT; }
	const std::string GetName() const override { return "RampageScript"; }
	int GetSortOrder() override;
	void SetRunTime(const float time);
	float GetRunTime();
	void SetRequiredKills(const unsigned int kills);
	unsigned int GetRequiredKills();
	unsigned int GetKills();
	void Activate(std::weak_ptr<Transform2D> targetTransform);
	bool IsActive();
	
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {

	}
private:
	bool activated = false, deactivated = false;
	float endTimer = -1.0f;
	float runTime = 50.0f, runTimerReset = 50.0f;
	void Reset();
	int kills = 0;
	unsigned int requiredKills = 40, killCountAtStart = 0, finalKillCount;
	Vector2 positionReset;
};


#endif // !WS_RAMPAGE_SCRIPT_H
