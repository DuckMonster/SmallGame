#pragma once
#include "Core/Input/Input.h"
struct ContextData;

enum class ContextEventType
{
	Invalid,
	Closed,
	KeyDown
};

struct Context
{
	ContextData* data;
	int width;
	int height;
	bool isOpen;
	bool isFocused;

	InputState input;
};

extern Context* gContext;

bool CreateContext();
void ContextUpdateFrame();
void ContextSwapBuffer();
void DestroyContext();