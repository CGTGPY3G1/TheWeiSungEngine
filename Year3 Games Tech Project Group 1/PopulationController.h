#pragma once
#ifndef WS_POPULATION_CONTROL_SCRIPT_H
#define WS_POPULATION_CONTROL_SCRIPT_H
#include "ScriptableComponent.h"
#include "AttackTypes.h"

struct KillData {
	AttackType killType;
	unsigned int victimId = 0;
	std::string victimName = "";
};

struct KillerData {
	unsigned int killerId = 0;
	std::string killerName = "";
	bool operator==(const KillerData & other) const {
		return (killerId == other.killerId && killerName.compare(other.killerName) == 0);
	}
};

namespace std {
	template <> struct hash<KillerData> { std::size_t operator()(const KillerData& data) const { return ((hash<string>()(data.killerName) ^ (hash<unsigned int>()(data.killerId) << 1)) >> 1); } };
}
struct ScoreTracker {
	unsigned int points = 0;
	std::vector<KillData> kills;
};
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
	unsigned int GetScore(const std::string & characterName);
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
	std::unordered_map<KillerData, ScoreTracker> killRegister;
};

#endif // !WS_POPULATION_CONTROL_SCRIPT_H