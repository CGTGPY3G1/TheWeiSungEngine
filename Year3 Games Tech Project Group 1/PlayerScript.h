#pragma once
#ifndef WS_PLAYER_SCRIPT_H
#define WS_PLAYER_SCRIPT_H
#include "ScriptableComponent.h"
#include <memory>
class VehicleController;
class GameObject;
class PlayerScript : public ScriptableComponent {
public:
	PlayerScript();
	PlayerScript(std::weak_ptr<GameObject> gameObject);
	~PlayerScript();
	const ComponentType Type() const override { return COMPONENT_PLAYER_SCRIPT; }
	void Start() override;
	void Update(const float & deltaTime) override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	const std::string GetName() const override { return "PlayerScript"; }
	int GetSortOrder() override;
private:
	bool driving = false;
	float reloadTime = 0.0f;
	std::weak_ptr<VehicleController> car;
	std::weak_ptr<GameObject> player;
};

#endif //! WS_PLAYER_SCRIPT_H