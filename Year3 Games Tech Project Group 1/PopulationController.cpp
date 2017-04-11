#include "PopulationController.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "ComponentHeaders.h"
#include "Engine.h"
#include "Graphics.h"
#include "Math.h"
#include "AttackerIdentityScript.h"
#include <algorithm>

PopulationController & PopulationController::GetInstance() {
	static PopulationController instance;
	return instance;
}
PopulationController::PopulationController() {
	map = GameObjectManager::GetInstance().GetGameObject("Tileset").lock()->GetComponent<TileMapper>();
}

PopulationController::~PopulationController() {
}

void PopulationController::Update() {
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Graphics> graphics = engine.GetGraphics().lock();
	spawnScale = graphics->GetCameraZoom();
	ManagePopulation();
}

unsigned int PopulationController::GetKills() {
	return kills;
}

float PopulationController::GetSpawnRadius() {
	return spawnRadius;
}

void PopulationController::RegisterKill(const AttackerInfo & attacker, const unsigned int & victimID, const std::string & victimName) {
	KillerData killerData;
	killerData.killerId = attacker.iD;
	killerData.killerName = attacker.name;
	KillData killData;
	killData.killType = attacker.attackType;
	killData.victimId = victimID;
	killData.victimName = victimName;
	killRegister[killerData].points += attacker.attackType == AttackType::VehicleHit ? 20 : (attacker.attackType == AttackType::GrenadeHit) ? 10 : 50;
	killRegister[killerData].kills.push_back(killData);
	std::cout << attacker.name + " killed " + victimName + " with " + (attacker.attackType == AttackType::VehicleHit ? "a Vehicle " : (attacker.attackType == AttackType::GrenadeHit) ? "an Explosion" : "a Bullet" ) + " Total Points = " + std::to_string(killRegister[killerData].points) << std::endl;
}

void PopulationController::SetSpawnRadius(const float & radius) {
	spawnRadius = radius;
}

void PopulationController::SetMaxCivs(const unsigned int & max) {
	maxCivs = max;
}

unsigned int PopulationController::GetMaxCivs() {
	return maxCivs;
}

unsigned int PopulationController::GetScore(const std::string & characterName) {
	for(std::unordered_map<KillerData, ScoreTracker>::iterator i = killRegister.begin(); i != killRegister.end(); ++i) {
		if((*i).first.killerName.compare(characterName) == 0) {
			return (*i).second.points;
		}
	}
	return 0;
}

std::weak_ptr<GameObject> PopulationController::GetRandomCiv() {
	const int index = Random::RandomInt(civs.size());
	return index < civs.size() ? civs[index].lock()->GetGameObject() : std::weak_ptr<GameObject>();
}

void PopulationController::ManagePopulation() {
	std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
	const unsigned int scaledMaxCivs = maxCivs * (spawnScale * spawnScale);
	if(civs.size() < scaledMaxCivs) SpawnCivilians(1);
	const Vector2 centre = graphics->GetCameraPosition();
	const float radius = (spawnRadius * spawnScale * 1.4f) * (spawnRadius * spawnScale * 1.4f);
	std::vector<std::weak_ptr<Transform2D>>::iterator it = civs.begin();
	while(it != civs.end()) {
		std::shared_ptr<Transform2D> civ = (*it).lock();
		if(civ) {
			if((civ->GetPosition() - centre).SquareMagnitude() > radius) {
				std::shared_ptr<GameObject> g = civ->GetGameObject().lock();
				if(civs.size() <= scaledMaxCivs) {
					MoveCivilian(civ);
					++it;
				}
				else {
					std::shared_ptr<GameObject> g = civ->GetGameObject().lock();
					if(g) g->Destroy();
					it = civs.erase(it);
				}
			}
			else {
				++it;
			}
		}
		else {
			it = civs.erase(it);
			kills++;
		}
	}
}

void PopulationController::SpawnCivilians(const unsigned int number) {
	std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
	for(size_t i = 0; i < number; i++) {
		Vector2 position;
		bool positionClear = false;
		const Vector2 offset = Vector2(32.0f, 32.0f);
		int tryCount = 0;
		while(tryCount < 3 && !positionClear) {
			tryCount++;
			const float angle = Random::RandomFloat(0.0f, Math::TAU());
			position = Vector2::Right;
			position.RotateInRadians(angle);
			const float dist = Random::RandomFloat((spawnRadius * spawnScale) / 2, spawnRadius * spawnScale);
			position *= dist;
			position += graphics->GetCameraPosition();
			std::shared_ptr<TileMapper> tileMap = map.lock();
			TileType tileType = tileMap->GetTileType(position);
			positionClear = ((tileType == TILE_TYPE_PAVEMENT || tileType == TILE_TYPE_GRASS || tileType == TILE_TYPE_SAND || tileType == TILE_TYPE_SAND_GRASS || tileType == TILE_TYPE_PAVEMENT_GRASS) ?
							 !PhysicsSystem::GetInstance().CheckAABB(AABB(position - offset, position + offset)) : false);
		}
		if(positionClear) {
			const float rotation = Random::RandomFloat(0.0f, 360.0f);
			civs.push_back(GameObjectFactory::CreateCharacter("Character", Random::RandomInt(1, 9), true, position, Vector2::One, rotation).lock()->GetComponent<Transform2D>());
		}
	}
}

void PopulationController::MoveCivilian(std::shared_ptr<Transform2D> targetTransform) {
	if(targetTransform) {
		std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
		Vector2 position;
		bool positionClear = false;
		const Vector2 offset = Vector2(32.0f, 32.0f);
		while(!positionClear) {
			const float angle = Random::RandomFloat(0.0f, Math::TAU());
			position = Vector2::Right;
			position.RotateInRadians(angle);
			const float dist = Random::RandomFloat((spawnRadius * spawnScale) / 2, spawnRadius * spawnScale);
			position *= dist;
			position += graphics->GetCameraPosition();
			std::shared_ptr<TileMapper> tileMap = map.lock();
			TileType tileType = tileMap->GetTileType(position);
			positionClear = ((tileType == TILE_TYPE_PAVEMENT || tileType == TILE_TYPE_GRASS || tileType == TILE_TYPE_SAND || tileType == TILE_TYPE_SAND_GRASS || tileType == TILE_TYPE_PAVEMENT_GRASS) ?
							 !PhysicsSystem::GetInstance().CheckAABB(AABB(position - offset, position + offset)) : false);
		}
		if(positionClear) {
			const float rotation = Random::RandomFloat(0.0f, 360.0f);
			std::shared_ptr<RigidBody2D> rb = targetTransform->GetComponent<RigidBody2D>().lock();
			rb->SetPosition(position);
			rb->SetRotation(rotation);
			targetTransform->GetComponent<CharacterScript>().lock()->Reset();
			targetTransform->GetComponent<HealthScript>().lock()->Reset();
		}
	}
}
