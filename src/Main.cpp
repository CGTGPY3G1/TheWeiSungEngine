#include "Engine.h"
#include "RunShootDrive.h"
#include "TestScene.h"

void main(int argA, char** argB) {
	Engine & engine = Engine::GetInstance();
	engine.Start();
	std::shared_ptr<Game> game = std::make_shared<RunShootDrive>();
	engine.SetGame(game);
	while(engine.IsRunning()) {
		engine.Run();
	}
}

