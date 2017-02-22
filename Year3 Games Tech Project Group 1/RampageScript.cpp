#include "RampageScript.h"
#include "GameObject.h"
#include "ComponentHeaders.h"
#include "GameObjectFactory.h"
#include "Math.h"
#include "PhysicsSystem.h"
#include "Engine.h"
#include "Graphics.h"
//#include <ios>
#include  <iomanip>
//#include <SFML/Graphics.hpp>

RampageScript::RampageScript() : ScriptableComponent() {
}

RampageScript::RampageScript(std::weak_ptr<GameObject> gameObject) : ScriptableComponent(gameObject) {
}

RampageScript::~RampageScript() {
}

void RampageScript::Start() {

}

void RampageScript::Update(const float & deltaTime) {
	const size_t noOfCivs = civs.size();
	if(activated) {
		ManagePopulation();
		bool rampageOver = kills >= requiredKills || runTime <= 0.0001f;
		if(!rampageOver) {
			runTime -= deltaTime;
		}
		else {
			if(!deactivated) {
				deactivated = true;
				finalKillCount = kills;
				endTimer = 10.0f;
			}
			if(endTimer > 0.0f) endTimer -= deltaTime;
			else Reset();
		}
	}
	else {
		std::vector<std::weak_ptr<Transform2D>>::iterator it = civs.begin();
		if(it != civs.end()) {
			std::shared_ptr<Transform2D> civ = (*it).lock();
			if(civ) {
				std::shared_ptr<GameObject> g = civ->GetGameObject().lock();
				if(g) g->Destroy();
			}
			it = civs.erase(it);
		}
	}
}

void RampageScript::Render() {
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Graphics> graphics = engine.GetGraphics().lock();
	spawnScale = graphics->GetCameraZoom();
	if(activated) {
		const float zoom = graphics->GetCameraZoom();
		const bool rampageOver = finalKillCount >= requiredKills || runTime <= 0.0001f;
		if(!rampageOver)   graphics->Draw("Kill Count = " + std::to_string(kills) + "	Required = " + std::to_string(requiredKills) + " |	Time = " + std::to_string(runTime), Vector2(100.0f, 650.0f), (unsigned int)(30.0f * spawnScale));
		else if(endTimer > 0.0f) {
			Vector2 position = Vector2(640.0f, 250.0f);
			graphics->Draw(((kills >= requiredKills) ? "You Win!" : "You Lose!"), position, (unsigned int)(80.0f * zoom), 1.0f, 1.0f, 1.0f, 1.0f, TextAlignment::CENTRE_ALIGNED);
			position.y += 100.0f;
			graphics->Draw("Kills = " + std::to_string(finalKillCount) + " / " + std::to_string(requiredKills), position, (unsigned int)(80.0f * zoom), 1.0f, 1.0f, 1.0f, 1.0f, TextAlignment::CENTRE_ALIGNED);
			position.y += 100.0f;
			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << (runTimerReset - std::max(runTime, 0.0f));
			graphics->Draw("Time Taken = " + ss.str() + " seconds", position, (unsigned int)(80.0f * zoom), 1.0f, 1.0f, 1.0f, 1.0f, TextAlignment::CENTRE_ALIGNED);
		}
	}
	else {
		std::shared_ptr<Transform2D> t = GetComponent<Transform2D>().lock();
		if(t) {
			const float radius = 32.0f;
			sf::CircleShape circle(radius);
			circle.setOrigin(radius, radius);
			sf::Color fillColour = TypeConversion::ConvertToSFColour(0.0f, 1.0f, 0.0f, 0.2f);
			circle.setFillColor(fillColour);
			circle.setOutlineThickness(1.f);
			sf::Color outlineColour = TypeConversion::ConvertToSFColour(0.0f, 1.0f, 0.0f, 1.0f);
			circle.setOutlineColor(outlineColour);
			Engine::GetInstance().GetGraphics().lock()->Draw(circle, t->GetWorldTransform());
		}
	}
}

void RampageScript::ManagePopulation() {
	std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
	const unsigned int maxCivs = MAX_CIVS * (spawnScale * spawnScale);
	if(civs.size() < maxCivs) SpawnCivilians(1);
	const Vector2 centre = graphics->GetCameraPosition();
	const float radius = (spawnRadius * spawnScale * 1.4f) * (spawnRadius * spawnScale * 1.4f);
	std::vector<std::weak_ptr<Transform2D>>::iterator it = civs.begin();
	while(it != civs.end()) {
		std::shared_ptr<Transform2D> civ = (*it).lock();
		if(civ) {
			if((civ->GetPosition() - centre).SquareMagnitude() > radius) {
				std::shared_ptr<GameObject> g = civ->GetGameObject().lock();
				if(civs.size() <= maxCivs) {
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

void RampageScript::SpawnCivilians(const unsigned int number) {
	std::shared_ptr<Graphics> graphics = Engine::GetInstance().GetGraphics().lock();
	for(size_t i = 0; i < number; i++) {
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
			positionClear =  !PhysicsSystem::GetInstance().CheckAABB(AABB(position - offset, position + offset));
		}
		const float rotation = Random::RandomFloat(0.0f, 360.0f);
		civs.push_back(GameObjectFactory::CreateCharacter("Character", 1, true, position, Vector2::One, rotation).lock()->GetComponent<Transform2D>());
	}
}

void RampageScript::MoveCivilian(std::shared_ptr<Transform2D> targetTransform) {
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
			positionClear = !PhysicsSystem::GetInstance().CheckAABB(AABB(position - offset, position + offset));
		}
		const float rotation = Random::RandomFloat(0.0f, 360.0f);
		std::shared_ptr<RigidBody2D> rb = targetTransform->GetComponent<RigidBody2D>().lock();
		rb->SetPosition(position);
		rb->SetRotation(rotation);
	}
	
}

void RampageScript::Reset() {
	target = std::weak_ptr<Transform2D>();
	runTime = runTimerReset;
	endTimer = -1.0f;
	kills = 0;
	finalKillCount = 0;
	activated = false;
	deactivated = false;
}

int RampageScript::GetSortOrder() {
	const static int order = TypeInfo::ScriptSortOrder<RampageScript>();
	return order;
}

void RampageScript::SetRunTime(const float time) {
	runTime = time;
	runTimerReset = time;
}

float RampageScript::GetRunTime() {
	return runTime;
}

void RampageScript::SetRequiredKills(const unsigned int kills) {
	requiredKills = kills;
}

unsigned int RampageScript::GetRequiredKills() {
	return 0.0f;
}

unsigned int RampageScript::GetKills() {
	return kills;
}

float RampageScript::GetSpawnRadius() {
	return spawnRadius;
}

void RampageScript::SetSpawnRadius(const float radius) {
	spawnRadius = radius;
}

void RampageScript::Activate(std::weak_ptr<Transform2D> targetTransform) {
	Reset();
	target = targetTransform.lock();
	activated = true;
}

bool RampageScript::IsActive() {
	return activated;
}
