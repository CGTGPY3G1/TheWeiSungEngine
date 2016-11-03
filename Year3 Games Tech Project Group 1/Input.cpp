#include "Input.h"
#include "TypeConversion.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include "Engine.h"

Input::Input() {
	for(int i = 0; i < NUMBER_OF_KEYS; i++) {
		keyStates[i] = false;
		keyChanged[i] = false;
	}

	for(int i = 0; i < NUMBER_OF_BUTTONS; i++) {
		buttonStates[i] = false;
		buttonChanged[i] = false;
	}
}

Input::~Input() {
}


void Input::Update() {
	std::lock_guard<std::mutex> lock(m);
	for(int i = 0; i < NUMBER_OF_KEYS; i++) {
		bool pressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
		if(keyStates[i] != pressed) {
			keyStates[i] = pressed;
			keyChanged[i] = true;
		}
		else if(keyChanged[i]) keyChanged[i] = false;
	}

	for(int i = 0; i < NUMBER_OF_BUTTONS; i++) {
		bool pressed = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
		if(buttonStates[i] != pressed) {
			buttonStates[i] = pressed;
			buttonChanged[i] = true;
		}
		else if(buttonChanged[i]) buttonChanged[i] = false;
	}
	Graphics * g = Engine::GetInstance().GetGraphics();
	Vector2 newMousePosition = TypeConversion::ConvertToVector2(g->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(g->GetWindow())));
	//Vector2 position = g->GetScreenPosition();
	//float height = g->GetScreenResolution().y;
	//newMousePosition -= position;
	//newMousePosition.y = height - newMousePosition.y;
	mousePosition.x = (float)newMousePosition.x; mousePosition.y = (float)newMousePosition.y;
}

bool Input::GetKeyDown(KeyCodes::KeyCode key) {
	std::lock_guard<std::mutex> lock(m);
	return keyStates[key] && keyChanged[key];
}

bool Input::GetKeyUp(KeyCodes::KeyCode key) {
	std::lock_guard<std::mutex> lock(m);
	return !keyStates[key] && keyChanged[key];
}

bool Input::GetKey(KeyCodes::KeyCode key) {
	std::lock_guard<std::mutex> lock(m);
	return keyStates[key];
}

bool Input::GetMouseButtonDown(MouseButtons::MouseButton key) {
	std::lock_guard<std::mutex> lock(m);
	return buttonStates[key] && buttonChanged[key];
}

bool Input::GetMouseButtonUp(MouseButtons::MouseButton key) {
	std::lock_guard<std::mutex> lock(m);
	return !buttonStates[key] && buttonChanged[key];
}

bool Input::GetMouseButton(MouseButtons::MouseButton key) {
	std::lock_guard<std::mutex> lock(m);
	return buttonStates[key];
}

Vector2 Input::GetMousePosition() {
	std::lock_guard<std::mutex> lock(m);
	return mousePosition;
}

float Input::GetMouseX() {
	std::lock_guard<std::mutex> lock(m);
	return mousePosition.x;
}

float Input::GetMouseY() {
	std::lock_guard<std::mutex> lock(m);
	return mousePosition.y;
}