#pragma once
#ifndef WS_TEST_SCENE_H
#define WS_TEST_SCENE_H

#include "Scene.h"
#include "Vector2.h"

class GameObject;
class RigidBody2D;
class TestScene : public Scene {
public:
	TestScene();
	~TestScene();
	void Detonate(Vector2 position, float radius, float explosionForce, float damage);
	void Start() override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	void Update(const float & deltaTime) override;
	void Render() override;
	void SetCamTarget(const std::weak_ptr<RigidBody2D> & camTarget);
private:
	std::weak_ptr<RigidBody2D> camTarget;
	Vector2 mousePosition;
	bool oldInputStyle = true;
	bool driving = false;
	bool printBMF = false;
	int playerHealthPercentage = 100;
	Vector2 playerPosition;
};

#endif // !WS_TEST_SCENE_H+