#pragma once
struct ContextData;

enum class Context_Event_Type
{
	Invalid,
	Closed,
	KeyDown
};

struct Context
{
	ContextData* data	= nullptr;
	int width			= 0;
	int height			= 0;
	bool is_open		= false;
	bool is_focused		= false;
};

extern Context context;

bool CreateContext();
void ContextUpdateFrame();
void ContextSwapBuffer();
void DestroyContext();