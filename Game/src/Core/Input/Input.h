#pragma once
#include "Key.h"
#include "Mouse.h"

struct ActionState
{
	bool pressed = false;
	int frameNum = -1;
};

struct KeyboardState
{
	ActionState keys[(uint16)Key::MAX];
	ActionState& operator[](Key key)
	{
		return keys[(uint16)key];
	}
	const ActionState& operator[](Key key) const
	{
		return keys[(uint16)key];
	}
};

struct MouseState
{
	int x;
	int y;
	float prevWheel;
	float wheel;

	ActionState buttons[(uint8)MouseButton::MAX];
	ActionState& operator[](MouseButton btn)
	{
		return buttons[(uint16)btn];
	}
	const ActionState& operator[](MouseButton btn) const
	{
		return buttons[(uint16)btn];
	}
};

struct InputState
{
	int inputFrame = 0;

	KeyboardState keyboardState;
	MouseState mouseState;

	bool GetKey(Key key) const
	{
		return keyboardState[key].pressed;
	}
	bool GetKeyPressed(Key key) const
	{
		return keyboardState[key].pressed && keyboardState[key].frameNum == inputFrame;
	}
	bool GetKeyReleased(Key key) const
	{
		return !keyboardState[key].pressed && keyboardState[key].frameNum == inputFrame;
	}

	bool GetMouseButton(MouseButton btn) const
	{
		return mouseState[btn].pressed;
	}
	bool GetMouseButtonPressed(MouseButton btn) const
	{
		return mouseState[btn].pressed && mouseState[btn].frameNum == inputFrame;
	}
	bool GetMouseButtonReleased(MouseButton btn) const
	{
		return !mouseState[btn].pressed && mouseState[btn].frameNum == inputFrame;
	}

	void SetKey(Key key, bool pressed)
	{
		keyboardState[key].pressed = pressed;
		keyboardState[key].frameNum = inputFrame;
	}
	void SetMouseButton(MouseButton btn, bool pressed)
	{
		mouseState[btn].pressed = pressed;
		mouseState[btn].frameNum = inputFrame;
	}

	float GetWheel()
	{
		return mouseState.wheel;
	}
	float GetWheelDelta()
	{
		return mouseState.wheel - mouseState.prevWheel;
	}
};
