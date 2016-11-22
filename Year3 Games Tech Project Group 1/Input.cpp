#include "Input.h"
#include "TypeConversion.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include "Engine.h"
#include <Xinput.h>
#pragma comment(lib,"Xinput9_1_0.lib")
//#ifdef MSC_VER < 1700 //pre 2012
//#pragma comment(lib,"Xinput.lib")
//#else
//#pragma comment(lib,"Xinput9_1_0.lib")
//#endif
Input::Input() {
	const bool FALSE_BOOL = false;
	for(unsigned int i = 0; i < NUMBER_OF_KEYS; i++) {
		keyStates[i] = FALSE_BOOL;
		keyChanged[i] = FALSE_BOOL;
	}

	for(unsigned int i = 0; i < NUMBER_OF_MOUSE_BUTTONS; i++) {
		mouseButtonStates[i] = FALSE_BOOL;
		mouseButtonChanged[i] = FALSE_BOOL;
	}
	for(unsigned int i = 0; i < NUMBER_OF_CONTROLLERS; i++) {
		usingController[i] = false;
		for(unsigned int j = 0; j < NUMBER_OF_CONTROLLER_BUTTONS; j++) {
			ControllerButtonStates[i][j] = FALSE_BOOL;
			ControllerButtonChanged[i][j] = FALSE_BOOL;
		}	
	}
	const float ZERO_FLOAT = 0.0f;
	for(unsigned int i = 0; i < NUMBER_OF_CONTROLLERS; i++) {
		for(unsigned int j = 0; j < NUMBER_OF_CONTROLLER_AXES; j++) {
			axisStates[i][j] = ZERO_FLOAT;
		}
	}
}

Input::~Input() {
}


void Input::Update() {
	std::lock_guard<std::mutex> lock(m);
	for(unsigned int i = 0; i < NUMBER_OF_KEYS; i++) {
		bool pressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
		if(keyStates[i] != pressed) {
			keyStates[i] = pressed;
			keyChanged[i] = true;
		}
		else if(keyChanged[i]) keyChanged[i] = false;
	}

	for(unsigned int i = 0; i < NUMBER_OF_MOUSE_BUTTONS; i++) {
		bool pressed = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
		if(mouseButtonStates[i] != pressed) {
			mouseButtonStates[i] = pressed;
			mouseButtonChanged[i] = true;
		}
		else if(mouseButtonChanged[i]) mouseButtonChanged[i] = false;
	}
	for(unsigned int i = 0; i < NUMBER_OF_CONTROLLERS; i++) {
		for(unsigned int j = 0; j < NUMBER_OF_CONTROLLER_BUTTONS; j++) {
			bool pressed = sf::Joystick::isButtonPressed(i, j);
			if(ControllerButtonStates[i][j] != pressed) {
				ControllerButtonStates[i][j] = pressed;
				ControllerButtonChanged[i][j] = true;
			}
			else if(ControllerButtonChanged[i][j]) ControllerButtonChanged[i][j] = false;
		}
	}
	
	for(unsigned int i = 0; i < NUMBER_OF_CONTROLLERS; i++) { 
		for(unsigned int j = 0; j < NUMBER_OF_CONTROLLER_AXES; j++) {
			axisStates[i][j] = GetAxisValue(i, j);
		}
	}
	
	Graphics * g = Engine::GetInstance().GetGraphics();
	Vector2 newMousePosition = TypeConversion::ConvertToVector2(g->GetWindow().mapPixelToCoords(sf::Mouse::getPosition(g->GetWindow())));
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

bool Input::GetMouseButtonDown(MouseButtons::MouseButton button) {
	std::lock_guard<std::mutex> lock(m);
	return mouseButtonStates[button] && mouseButtonChanged[button];
}

bool Input::GetMouseButtonUp(MouseButtons::MouseButton button) {
	std::lock_guard<std::mutex> lock(m);
	return !mouseButtonStates[button] && mouseButtonChanged[button];
}

bool Input::GetMouseButton(MouseButtons::MouseButton button) {
	std::lock_guard<std::mutex> lock(m);
	return mouseButtonStates[button];
}

bool Input::GetControllerButtonDown(unsigned int controllerID, ControllerButtons::ControllerButton button) {
	std::lock_guard<std::mutex> lock(m);
	return ControllerButtonStates[controllerID][button] && ControllerButtonChanged[controllerID][button];
}

bool Input::GetControllerButtonUp(unsigned int controllerID, ControllerButtons::ControllerButton button) {
	std::lock_guard<std::mutex> lock(m);
	return !ControllerButtonStates[button] && ControllerButtonChanged[button];
}

bool Input::GetControllerButton(unsigned int controllerID, ControllerButtons::ControllerButton button) {
	std::lock_guard<std::mutex> lock(m);
	return ControllerButtonStates[controllerID][button];
}

bool Input::IsControllerConnected(unsigned int controllerID) {
	return sf::Joystick::isConnected(controllerID);
}

float Input::GetAxis(unsigned int controllerID, ControllerButtons::ControllerAxis axis) {
	return axisStates[controllerID][axis];
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

void Input::SetControllerActive(const unsigned int controllerID, const bool & useController) {
	if(IsControllerConnected(controllerID)) usingController[controllerID] = useController;
}

bool Input::GetControllerActive(const unsigned int controllerID) {
	return usingController[controllerID];
}

float Input::GetAxisValue(const unsigned int controllerID, const unsigned int axisID) {
	float val = 0.0f;
	switch(axisID) {
	case ControllerButtons::ControllerAxis::LT:
	case ControllerButtons::ControllerAxis::RT:
		XINPUT_STATE controllerState;
		ZeroMemory(&controllerState, sizeof(XINPUT_STATE));
		XInputGetState(controllerID, &controllerState);
		if(axisID == ControllerButtons::ControllerAxis::LT) {
			val = (float)controllerState.Gamepad.bLeftTrigger * (1.0f / 255.0f);
		}
		else {
			val = (float)controllerState.Gamepad.bRightTrigger * (1.0f / 255.0f);
		}
		break;
	case ControllerButtons::ControllerAxis::DPadH:
	case ControllerButtons::ControllerAxis::DPadV:
	case ControllerButtons::ControllerAxis::LeftStickH:
	case ControllerButtons::ControllerAxis::LeftStickV:
	case ControllerButtons::ControllerAxis::RightStickH:
	case ControllerButtons::ControllerAxis::RightStickV:
		val = sf::Joystick::getAxisPosition(controllerID, (sf::Joystick::Axis)axisID) * 0.01f;
		break;
	default:
		break;
	}
	if(val > -DEAD_ZONE && val < DEAD_ZONE) val = 0.0f;
	else val = (val - ( val > 0.0f ? DEAD_ZONE : -DEAD_ZONE)) / (1.0f - DEAD_ZONE);
	return val;
}
