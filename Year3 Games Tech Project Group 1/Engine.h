#pragma once
#ifndef WS_ENGINE_H
#define WS_ENGINE_H

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
	void Init();
	void Run();
	void Close();
	bool IsRunning();
	Graphics * GetGraphics();
	DebugDraw * GetDebugDraw();
	Input * GetInput();
	Time * GetTimer();
	FileIO * GetFileIO();
private:
	Engine();
	Graphics * graphics;
	Input * input;
	Time * timer;
	FileIO * fileIO;
	Game * game;
	DebugDraw * debugDraw;
	bool running;
};


#endif // !WS_ENGINE_H
