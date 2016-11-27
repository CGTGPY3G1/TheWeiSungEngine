#pragma once
#ifndef WS_GRAPHICS_H
#define WS_GRAPHICS_H

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include "Vector2.h"
#include <cstdarg>
#include <memory>
enum TextAlignment {
	LEFT_ALIGNED = 0,
	CENTRE_ALIGNED,
	RIGHT_ALIGNED
};

struct GraphicsSettings {
	std::string windowTitle = "Unnamed Window";
	Vector2 screenPosition = {500.0f, 20.0f}, resolution = {1280.0f, 720.0f};
	unsigned int depthBits = 24, antialiasingLevel = 0, minorVersion = 2, majorversion = 3, maxFPS = 50;
	bool fullScreen = false, resizeable = true, vSync = false;
	 
	bool operator == (GraphicsSettings other) {
		return (screenPosition == screenPosition && resolution == resolution && depthBits == other.depthBits && antialiasingLevel == other.antialiasingLevel && minorVersion == other.minorVersion &&
				majorversion == other.majorversion && maxFPS == other.maxFPS && fullScreen == other.fullScreen && resizeable == other.resizeable && vSync == other.vSync && windowTitle == other.windowTitle);
	}
	bool operator != (GraphicsSettings other) {
		return !((*this) == other);
	}
};

class Graphics : public std::enable_shared_from_this<Graphics> {
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
	void Display();
	void Draw(const sf::Sprite & sprite);
	void Draw(const sf::Shape & shape);
	void Draw(const sf::VertexArray & vertexArray);
	void Draw(const std::string & text, const Vector2 & position, unsigned int characterSize, TextAlignment alignment = TextAlignment::LEFT_ALIGNED);
	void DrawCircle(const sf::CircleShape & c);
	void Draw(const sf::Drawable & d, const sf::Transform & t);
	void DrawCircle(const Vector2 & position, const float & radius, bool filled = false, const float & r = 1.0f, const float & g = 1.0f, const float & b = 1.0f, const float & a = 1.0f, const unsigned int & noOfVerts = 32);
	void DrawRect(const sf::RectangleShape & r);
	void DrawRect(const Vector2 & position, const Vector2 & size, const float & rotation = 0.0f, bool filled = false, const float & r = 1.0f, const float & g = 1.0f, const float & b = 1.0f, const float & a = 1.0f);
	void DrawLine(sf::Vertex line []);
	void SetDepth(const unsigned int & depth);
	Vector2 GetCameraPosition();
	void SetCameraPosition(const float & x, const float & y);
	void SetCameraPosition(const Vector2 & position);
	void MoveCamera(const float & x, const float & y);
	void MoveCamera(const Vector2 & amount);
	void SetCameraZoom(const float & zoom);
	float GetCameraZoom();
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
	std::weak_ptr<Graphics> GetWeak() { return shared_from_this(); }
	sf::RenderWindow window;
	sf::View & view = sf::View();
	sf::ContextSettings sfSettings;
	sf::Font font;
	GraphicsSettings settings;
	bool windowOpen = false;
	sf::VertexArray buffer;
	sf::Vector2f camPos = sf::Vector2f(0.0f, 0.0f);
	float zoomLevel = 1.0f;
};

#endif // !WS_GRAPHICS_H
