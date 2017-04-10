#pragma once
#ifndef WS_POPULATION_CONTROL_SCRIPT_H
#define WS_POPULATION_CONTROL_SCRIPT_H

#include "ScriptableComponent.h"

struct KillerData {
	unsigned int killerId = 0;
	std::string killerName = "";
};
namespace std {
	template <> struct hash<KillerData> { std::size_t operator()(const KillerData& data) const { return ((hash<string>()(data.killerName) ^ (hash<unsigned int>()(data.killerId) << 1)) >> 1); } };
}
struct KillData;
struct AttackerInfo;
class TileMapper;
class Transform2D;
class PopulationController {
public:
	static PopulationController & GetInstance();
	
	~PopulationController();
	void Update();
	unsigned int GetKills();
	float GetSpawnRadius();
	void RegisterKill(const AttackerInfo & attacker, const unsigned int & victimID, const std::string & victimName);
	void SetSpawnRadius(const float & radius);
	void SetMaxCivs(const unsigned int & max);
	unsigned int GetMaxCivs();
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
	std::unordered_map<KillerData, std::vector<KillData>> killRegister;
};

#endif // !WS_POPULATION_CONTROL_SCRIPT_H