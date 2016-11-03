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

class Input {

public:
	Input();
	~Input();
	void Update();
	bool GetKeyDown(KeyCodes::KeyCode key);
	bool GetKeyUp(KeyCodes::KeyCode key);
	bool GetKey(KeyCodes::KeyCode key);
	bool GetMouseButtonDown(MouseButtons::MouseButton key);
	bool GetMouseButtonUp(MouseButtons::MouseButton key);
	bool GetMouseButton(MouseButtons::MouseButton key);
	Vector2 GetMousePosition();
	float GetMouseX();
	float GetMouseY();
private:
	const int NUMBER_OF_BUTTONS = MouseButtons::MouseButton::ButtonCount, NUMBER_OF_KEYS = KeyCodes::KeyCode::KeyCount;
	bool keyStates[KeyCodes::KeyCode::KeyCount] = {0};
	bool keyChanged[KeyCodes::KeyCode::KeyCount] = {0};

	bool buttonStates[MouseButtons::MouseButton::ButtonCount];
	bool buttonChanged[MouseButtons::MouseButton::ButtonCount];
	Vector2 mousePosition;
	std::mutex m;
};



#endif // !WS_INPUT_H