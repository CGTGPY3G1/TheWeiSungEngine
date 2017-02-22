#include "RampageScript.h"
#include "GameObject.h"
#include "ComponentHeaders.h"
#include "PopulationController.h"
#include "Math.h"

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
			kills = PopulationController::GetInstance().GetKills() - killCountAtStart;
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
}

void RampageScript::Render() {
	Engine engine = Engine::GetInstance();
	std::shared_ptr<Graphics> graphics = engine.GetGraphics().lock();
	if(activated) {
		const float zoom = graphics->GetCameraZoom();
		const bool rampageOver = finalKillCount >= requiredKills || runTime <= 0.0001f;
		if(!rampageOver)   graphics->Draw("Kill Count = " + std::to_string(kills) + "	Required = " + std::to_string(requiredKills) + " |	Time = " + std::to_string(runTime), Vector2(100.0f, 650.0f), (unsigned int)(30.0f * zoom));
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

void RampageScript::Reset() {
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

void RampageScript::Activate(std::weak_ptr<Transform2D> targetTransform) {
	Reset();
	killCountAtStart = PopulationController::GetInstance().GetKills();
	activated = true;
}

bool RampageScript::IsActive() {
	return activated;
}
