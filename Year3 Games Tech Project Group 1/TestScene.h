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
	std::weak_ptr<GameObject> CreateBuilding(const int & buildingNumber, const Vector2 & position = Vector2(), const Vector2 & scale = Vector2(1.0f, 1.0f), const float & rotation = 0.0f);
	std::weak_ptr<GameObject> CreateCharacter(const std::string & name, const int & characterType, const Vector2 & position = Vector2(), const Vector2 & scale = Vector2(1.0f, 1.0f), const float & rotation = 0.0f);
	void Start() override;
	void FixedUpdate(const float & fixedDeltaTime) override;
	void Update(const float & deltaTime) override;
	void Render() override;
	//void SetUpShapes();
	void Test(const float & deltaTime);
private:
	std::weak_ptr<GameObject> g1, g2;
	Vector2 mousePosition;
	bool oldInputStyle = true;
};



#endif // !WS_TEST_SCENE_H