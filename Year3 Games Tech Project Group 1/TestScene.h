#pragma once
#ifndef WS_TEST_SCENE_H
#define WS_TEST_SCENE_H

#include "Scene.h"
#include "Vector2.h"

class GameObject;
class TestScene : public Scene {
public:
	TestScene();
	~TestScene();
	void Start() override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	void Update(const float & deltaTime) override;
	void Render() override;
private:
	Vector2 mousePosition;
	bool oldInputStyle = true;
	bool driving = false;
};

#endif // !WS_TEST_SCENE_H+