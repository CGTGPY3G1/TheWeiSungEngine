#pragma once
#ifndef WS_ENGINE_H
#define WS_ENGINE_H
#include <memory>
class Graphics;
class Input;
class Time;
class FileIO;
class Game;
class DebugDraw;
class Engine {
public:

	~Engine();
	static Engine & GetInstance();
	void Start();
	void Run();
	void End();
	void Close();
	bool IsRunning();
	void SetGame(std::shared_ptr<Game> newGame);
	Graphics * GetGraphics();
	DebugDraw * GetDebugDraw();
	Input * GetInput();
	Time * GetTimer();
	unsigned int GetFPS();

private:
	Engine();
	Graphics * graphics;
	Input * input;
	Time * timer;
	std::shared_ptr<Game> game;
	DebugDraw * debugDraw;
	bool running;
	unsigned int fps = 0, framesThisSecond = 0;
	float fpsTimer = 0.0f;
};


#endif // !WS_ENGINE_H
