#include "RunShootDrive.h"
#include "TestScene.h"
namespace WeiSungEngine {
	RunShootDrive::RunShootDrive() : Game() {

	}

	RunShootDrive::~RunShootDrive() {
	}

	void RunShootDrive::Start() {
		Game::Start();
		std::shared_ptr<Scene> scene = std::make_shared<TestScene>();
		AddScene(scene);
		SetActiveScene(0);
	}
}
