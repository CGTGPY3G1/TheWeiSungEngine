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
	if(activated) {		
		bool rampageOver = kills >= requiredKills || runTime <= 0.0001f;
		if(!rampageOver) {
				runTime -= deltaTime;
				if(civs.size() < MAX_CIVS) SpawnCivilians(1);
				if(!target.expired()) {
					std::shared_ptr<Transform2D> t = target.lock();
					if(t) {
						const Vector2 centre = t->GetPosition();
						float radius = (spawnRadius * 1.4f) * (spawnRadius * 1.4f);
						std::vector<std::weak_ptr<Transform2D>>::iterator it = civs.begin();
						while(it != civs.end()) {
							std::shared_ptr<Transform2D> civ = (*it).lock();
							if(civ) {
								if((civ->GetPosition() - centre).SquareMagnitude() > radius) {
									std::shared_ptr<GameObject> g = civ->GetGameObject().lock();
									if(g) g->Destroy();
									it = civs.erase(it);
								}
								else ++it;
							}
							else {
								it = civs.erase(it);
								kills++;
							}
						}
					}
				}
		}
		else {
			if(!deactivated) {
				deactivated = true;
				endTimer = 10.0f;
			}
			if(endTimer > 0.0f) endTimer -= deltaTime;
			else {
				std::vector<std::weak_ptr<Transform2D>>::iterator it = civs.begin();
				while(it != civs.end()) {
					std::shared_ptr<Transform2D> civ = (*it).lock();
					if(civ) {
						std::shared_ptr<GameObject> g = civ->GetGameObject().lock();
						if(g) g->Destroy();
					}
					it = civs.erase(it);
				}
				Reset();
			}
		}
	}
}

void RampageScript::Render() {
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Graphics> graphics = engine.GetGraphics().lock();
	float zoom = graphics->GetCameraZoom();
	if(activated)  {
		bool rampageOver = kills >= requiredKills || runTime <= 0.0001f;
		if(!rampageOver)   graphics->Draw("Kill Count = " + std::to_string(kills) + "	Required = " + std::to_string(requiredKills) + " |	Time = " + std::to_string(runTime), Vector2(100.0f, 650.0f), (unsigned int)(30.0f * zoom)); 
		else if(endTimer > 0.0f) {
			Vector2 position = Vector2(640.0f, 150.0f);
			graphics->Draw(((kills >= requiredKills) ? "You Win!" : "You Lose!"), position, (unsigned int)(80.0f * zoom), 1.0f, 1.0f, 1.0f, 1.0f, TextAlignment::CENTRE_ALIGNED);
			position.y += 100.0f;
			graphics->Draw("Kill Count = " + std::to_string(kills), position, (unsigned int)(80.0f * zoom), 1.0f, 1.0f, 1.0f, 1.0f, TextAlignment::CENTRE_ALIGNED);
			position.y += 100.0f;
			graphics->Draw("Target Kills = " + std::to_string(requiredKills), position, (unsigned int)(80.0f * zoom), 1.0f, 1.0f, 1.0f, 1.0f, TextAlignment::CENTRE_ALIGNED);
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

void RampageScript::SpawnCivilians(const unsigned int number) {
	std::shared_ptr<Transform2D> t = target.lock();
	if(t) {
		for(size_t i = 0; i < number; i++) {
			Vector2 position;
			bool positionClear = false;
			const Vector2 offset = Vector2(32.0f, 32.0f);
			while(!positionClear) {
				const float angle = Random::RandomFloat(0.0f, Math::TAU());
				position = Vector2::Right;
				position.RotateInRadians(angle);
				const float dist = Random::RandomFloat(spawnRadius / 2, spawnRadius);
				position *= dist;
				position += t->GetPosition();
				positionClear = !PhysicsSystem::GetInstance().CheckAABB(AABB(position - offset, position + offset));
			}
			const float rotation = Random::RandomFloat(0.0f, 360.0f);
			civs.push_back(GameObjectFactory::CreateCharacter("Character", 1, true, position, Vector2::One, rotation).lock()->GetComponent<Transform2D>());
		}
	}
	else std::cout << "No Target Transform" << std::endl;
}

void RampageScript::Reset() {
	target = std::weak_ptr<Transform2D>();
	runTime = runTimerReset;
	endTimer = -1.0f;
	kills = 0;
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

float RampageScript::GetRequiredKills() {
	return 0.0f;
}

float RampageScript::GetKills() {
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
