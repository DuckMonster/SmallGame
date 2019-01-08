#pragma once

class Time
{
public:
	// Initializes the time system (must be called before any other functionality)
	static void Init();

	// Updates the delta time (call at the beginning of a frame)
	static void Update();

	// Delta seconds of this frame
	static float Delta();

	// Total duration of the programs
	static float Duration();

	// Real frame timings (for checking how much the actual game is processing)
	static void BeginFrame();
	static void EndFrame();
	static float InternalFrameTime();
};
