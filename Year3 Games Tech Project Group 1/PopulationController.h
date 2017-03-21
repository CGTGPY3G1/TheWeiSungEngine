#pragma once
#ifndef WS_POPULATION_CONTROL_SCRIPT_H
#define WS_POPULATION_CONTROL_SCRIPT_H

#include "ScriptableComponent.h"

struct KillData {
	unsigned int killerId = 0;
	std::string killerName = "";
};

class TileMapper;
class Transform2D;
class PopulationController {
public:
	static PopulationController & GetInstance();
	
	~PopulationController();
	void Update();
	unsigned int GetKills();
	float GetSpawnRadius();
	void SetSpawnRadius(const float & radius);
	void SetMaxCivs(const unsigned int & max);
	unsigned int GetMaxCivs();
	template <class Archive>
	void load(Archive & ar) {

	}

	template <class Archive>
	void save(Archive & ar) const {

	}
private:
	PopulationController();
	int maxCivs = 10;
	float spawnRadius = 15 * Physics::PIXELS_PER_METRE, spawnScale = 1.0f;
	int kills = 0;
	std::weak_ptr<TileMapper> map;
	void ManagePopulation();
	void SpawnCivilians(const unsigned int number = 1);
	void MoveCivilian(std::shared_ptr<Transform2D> targetTransform);
	std::vector<std::weak_ptr<Transform2D>> civs;
};

#endif // !WS_POPULATION_CONTROL_SCRIPT_H