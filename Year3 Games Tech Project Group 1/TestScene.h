#pragma once
#ifndef WS_TEST_SCENE_H
#define WS_TEST_SCENE_H
#include "Scene.h"
class GameObject;
class TestScene : public Scene {
public:
	TestScene();
	~TestScene();
	void Start() override;
	void Update(const float & deltaTime) override;
	void SetUpShapes();
	void Test(const float & deltaTime);
private:
	std::weak_ptr<GameObject> g1, g2;
	
};



#endif // !WS_TEST_SCENE_H