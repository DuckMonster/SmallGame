#pragma once
#include "Key.h"
#include "Mouse.h"

struct ActionState
{
	bool pressed = false;
	int frame_num = -1;
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
	float prev_wheel;
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
	int input_frame = 0;

	KeyboardState keyboard_state;
	MouseState mouse_state;

	bool GetKey(Key key) const
	{
		return keyboard_state[key].pressed;
	}
	bool GetKeyPressed(Key key) const
	{
		return keyboard_state[key].pressed && keyboard_state[key].frame_num == input_frame;
	}
	bool GetKeyReleased(Key key) const
	{
		return !keyboard_state[key].pressed && keyboard_state[key].frame_num == input_frame;
	}

	bool GetMouseButton(MouseButton btn) const
	{
		return mouse_state[btn].pressed;
	}
	bool GetMouseButtonPressed(MouseButton btn) const
	{
		return mouse_state[btn].pressed && mouse_state[btn].frame_num == input_frame;
	}
	bool GetMouseButtonReleased(MouseButton btn) const
	{
		return !mouse_state[btn].pressed && mouse_state[btn].frame_num == input_frame;
	}

	void SetKey(Key key, bool pressed)
	{
		keyboard_state[key].pressed = pressed;
		keyboard_state[key].frame_num = input_frame;
	}
	void SetMouseButton(MouseButton btn, bool pressed)
	{
		mouse_state[btn].pressed = pressed;
		mouse_state[btn].frame_num = input_frame;
	}

	float GetWheel()
	{
		return mouse_state.wheel;
	}
	float GetWheelDelta()
	{
		return mouse_state.wheel - mouse_state.prev_wheel;
	}
};
