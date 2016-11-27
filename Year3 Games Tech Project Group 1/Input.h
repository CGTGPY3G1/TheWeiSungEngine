#pragma once
#ifndef WS_INPUT_H
#define WS_INPUT_H
#include "Vector2.h"
#include <mutex>
namespace KeyCodes {
	enum KeyCode {
		A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
		Escape, LControl, LShift, LAlt, LSystem, RControl, RShift, RAlt, RSystem, Menu, LBracket, RBracket, SemiColon, Comma, Period, Quote, Slash, BackSlash, Tilde,
		Equal, Dash, Space, Return, BackSpace, Tab, PageUp, PageDown, End, Home, Insert, Delete, Add, Subtract, Multiply, Divide, Left, Right, Up, Down, Numpad0, Numpad1,
		Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, Pause, KeyCount
	};
}

namespace MouseButtons {
	enum MouseButton {
		Left = 0, Right, Middle, XButton1, XButton2, ButtonCount
	};
}

namespace ControllerButtons {
	enum ControllerButton {
		A = 0, B, X, Y, LB, RB, BACK, START, LS, RS, ButtonCount
	};
	enum ControllerAxis {
		LeftStickH = 0, LeftStickV = 1, RT = 2, RightStickV = 3, RightStickH = 4, LT = 5, DPadH = 6, DPadV = 7, AxisCount = 8
	};
}

class Input : public std::enable_shared_from_this<Input> {

public:
	Input();
	~Input();
	void Update();
	bool GetKeyDown(KeyCodes::KeyCode key);
	bool GetKeyUp(KeyCodes::KeyCode key);
	bool GetKey(KeyCodes::KeyCode key);
	bool GetMouseButtonDown(MouseButtons::MouseButton button);
	bool GetMouseButtonUp(MouseButtons::MouseButton button);
	bool GetMouseButton(MouseButtons::MouseButton button);
	bool GetControllerButtonDown(unsigned int controllerID, ControllerButtons::ControllerButton button);
	bool GetControllerButtonUp(unsigned int controllerID, ControllerButtons::ControllerButton button);
	bool GetControllerButton(unsigned int controllerID, ControllerButtons::ControllerButton button);
	bool IsControllerConnected(unsigned int controllerID);
	float GetAxis(unsigned int controllerID, ControllerButtons::ControllerAxis axis);
	Vector2 GetMousePosition();
	float GetMouseX();
	float GetMouseY();
	void SetControllerActive(const unsigned int controllerID, const bool & useController);
	bool GetControllerActive(const unsigned int controllerID);
private:
	float GetAxisValue(const unsigned int controllerID, const unsigned int axisID);
	const unsigned int NUMBER_OF_MOUSE_BUTTONS = MouseButtons::MouseButton::ButtonCount, NUMBER_OF_CONTROLLER_BUTTONS = ControllerButtons::ControllerButton::ButtonCount, 
						NUMBER_OF_CONTROLLER_AXES = ControllerButtons::ControllerAxis::AxisCount, NUMBER_OF_KEYS = KeyCodes::KeyCode::KeyCount;
	const unsigned int NUMBER_OF_CONTROLLERS = 8;
	bool keyStates[KeyCodes::KeyCode::KeyCount] = {0};
	bool keyChanged[KeyCodes::KeyCode::KeyCount] = {0};

	bool mouseButtonStates[MouseButtons::MouseButton::ButtonCount];
	bool mouseButtonChanged[MouseButtons::MouseButton::ButtonCount];

	bool ControllerButtonStates[8][ControllerButtons::ControllerButton::ButtonCount];
	bool ControllerButtonChanged[8][ControllerButtons::ControllerButton::ButtonCount];

	float axisStates[8][ControllerButtons::ControllerAxis::AxisCount];
	const float ROUNDING_RANGE = 0.0001f, DEAD_ZONE = 0.2f;
	Vector2 mousePosition;
	std::mutex m;
	bool usingController[8];

	std::weak_ptr<Input> GetWeak() { return shared_from_this(); }
};



#endif // !WS_INPUT_H