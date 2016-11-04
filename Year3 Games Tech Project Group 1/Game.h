#pragma once
#ifndef WS_GAME_H
#define WS_GAME_H
#include <vector>
class Scene;
class Game {
public:
	Game();
	~Game();
	bool IsRunning();
	bool GameOver();
	void Start();
	void Update(const float & deltaTime);
	void Close();
	void AddScene(Scene * scene);
	unsigned int GetActiveScene();
	void SetActiveScene(unsigned int index);
private:
	bool running = false;
	bool gameOver = false;
	std::vector<Scene *> scenes;
	unsigned int activeScene;
};


#endif // !WS_GAME_H
