#pragma once
#ifndef WS_GRAPHICS_H
#define WS_GRAPHICS_H

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include "Vector2.h"
#include <cstdarg>
enum TextAlignment {
	LEFT_ALIGNED = 0,
	CENTRE_ALIGNED,
	RIGHT_ALIGNED
};

struct GraphicsSettings {
	std::string windowTitle = "Unnamed Window";
	Vector2 screenPosition = Vector2(100.0f, 50.0f), resolution = Vector2(800.0f, 600.0f);
	unsigned int depthBits = 24, antialiasingLevel = 0, minorVersion = 2, majorversion = 3, maxFPS = 60;
	bool fullScreen = false, resizeable = false, vSync = false;
	 
	bool operator == (GraphicsSettings other) {
		return (depthBits == other.depthBits && antialiasingLevel == other.antialiasingLevel && minorVersion == other.minorVersion &&
				majorversion == other.majorversion && maxFPS == other.maxFPS && fullScreen == other.fullScreen &&
				resizeable == other.resizeable && vSync == other.vSync && windowTitle == other.windowTitle);
	}
	bool operator != (GraphicsSettings other) {
		return !((*this) == other);
	}
};

class Graphics {
public:
	friend class sf::Text;
	Graphics();
	~Graphics();
	void Update();
	void End();
	void SetWindowTitle(const std::string & title);
	std::string GetWindowTitle();
	void SetScreenResolution(const float & width, const float & height);
	Vector2 GetScreenResolution();
	float GetScreenX();
	float GetScreenY();
	void SetScreenPosition(const float & x, const float & y);
	Vector2 GetScreenPosition();
	void Clear();

	void Draw(sf::Sprite sprite);
	void Draw(const sf::Shape & shape);
	void Draw(sf::VertexArray vertexArray);
	void Draw(const std::string & text, const Vector2 & position, unsigned int characterSize, TextAlignment alignment = TextAlignment::LEFT_ALIGNED);
	void DrawCircle(const sf::CircleShape & c);
	void Draw(const sf::Drawable & d, const sf::Transform & t);
	void DrawCircle(const Vector2 & position, const float & radius, bool filled = false, const float & r = 1.0f, const float & g = 1.0f, const float & b = 1.0f, const float & a = 1.0f, const unsigned int & noOfVerts = 32);
	void DrawRect(const sf::RectangleShape & r);
	void DrawRect(const Vector2 & position, const Vector2 & size, const float & rotation = 0.0f, bool filled = false, const float & r = 1.0f, const float & g = 1.0f, const float & b = 1.0f, const float & a = 1.0f);
	void DrawLine(sf::Vertex line []);

	void SetDepth(const unsigned int & depth);
	void MoveCamera(const float & x, const float & y) {
		view.move(sf::Vector2f(x, y));
		window.setView(view);
	}
	unsigned int GetDepth();
	void SetAntialiasingLevel(const unsigned int & antiAliasingLevel);
	unsigned int GetAntialiasingLevel();
	void SetFullScreen(const bool & fullScreen);
	bool GetFullScreen();
	void SetResizeable(const bool & resizeable);
	bool GetResizeable();
	void SetVSync(const bool & enabled);
	bool GetVSync();
	void SetMaxFPS(const unsigned int & maxFPS);
	unsigned int GetMaxFPS();
	GraphicsSettings GetSettings();
	void SetSettings(GraphicsSettings settings);
	sf::RenderWindow & GetWindow();
	HWND GetWindowHandle();
	bool GetWindowOpen();
private:
	void RebuildDisplay();
	sf::RenderWindow window;
	sf::View view;
	sf::ContextSettings sfSettings;
	sf::Font font;
	GraphicsSettings settings;
	bool windowOpen = false;
	sf::VertexArray buffer;
};

#endif // !WS_GRAPHICS_H
