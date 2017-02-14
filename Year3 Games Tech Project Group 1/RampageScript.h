#pragma once
#ifndef WS_REVOLUTE_JOINT_H
#define WS_REVOLUTE_JOINT_H

#include "ScriptableComponent.h"
class RigidBody2D;
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
	float GetSpawnRadius();
	void SetSpawnRadius(const float radius);
	void Activate(std::weak_ptr<Transform2D> targetTransform);
	bool IsActive();
	
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {

	}
private:
	void ManagePopulation();
	void SpawnCivilians(const unsigned int number = 1);
	void MoveCivilian(std::shared_ptr<Transform2D> targetTransform);
	std::weak_ptr<Transform2D> target;
	std::vector<std::weak_ptr<Transform2D>> civs;
	const unsigned int MAX_CIVS = 40;
	bool activated = false, deactivated = false;
	float endTimer = -1.0f;
	float runTime = 50.0f, runTimerReset = 50.0f;
	void Reset();
	unsigned int requiredKills = 40, kills = 0, finalKillCount;
	float spawnRadius = 35 * Physics::PIXELS_PER_METRE, spawnScale = 1.0f;
	Vector2 positionReset;
};


#endif // !WS_REVOLUTE_JOINT_H
