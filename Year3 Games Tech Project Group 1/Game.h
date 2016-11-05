#pragma once
#ifndef WS_GAME_H
#define WS_GAME_H
#include <vector>
#include <memory>
class Scene;
class Game : public std::enable_shared_from_this<Game> {
public:
	Game();
	virtual ~Game();
	bool IsRunning();
	bool GameOver();
	virtual void Start();
	virtual void Update(const float & deltaTime);
	virtual void Close();
	void AddScene(std::weak_ptr<Scene> scene);
	int GetActiveScene();
	void SetActiveScene(unsigned int index);
protected:
	std::weak_ptr<Game> GetWeak() { return shared_from_this(); }
	bool running = false;
	bool gameOver = false;
	std::vector<std::shared_ptr<Scene>> scenes;
	int activeScene = -1;
};


#endif // !WS_GAME_H
