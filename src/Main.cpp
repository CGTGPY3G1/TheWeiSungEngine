#include "Engine.h"
#include "RunShootDrive.h"
#include "TestScene.h"

void main(int argA, char** argB) {
	WeiSungEngine::Engine & engine = WeiSungEngine::Engine::GetInstance();
	engine.Start();
	std::shared_ptr<WeiSungEngine::Game> game = std::make_shared<WeiSungEngine::RunShootDrive>();
	engine.SetGame(game);
	while(engine.IsRunning()) {
		engine.Run();
	}
}

