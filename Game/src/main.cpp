
#include <GL/glew.h>
#include "Core/Context/Context.h"
#include "Core/Scene/Scene.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Memory/TempStack.h"
#include "Core/Memory/Allocator.h"
#include "Core/Debug/GLDebug.h"
#include "Core/Time/TimeStamp.h"
#include "Engine/Debug/DebugPrintManager.h"
#include "Engine/Debug/DebugDrawManager.h"
#include <windows.h>

#ifdef TEST
#include "Core/Test/Test.h"
int main()
{
	/* Test main starts here, only run tests */

	// Initialize temporary storage
	TempStack temp_stack;
	gTempStack = &temp_stack;

	// Run tests
	Test::Run();
}
#else
int main(int argc, char* argv)
{
	// Initialize temporary storage
	TempStack temp_stack;
	temp_stack.Resize(8);
	gTempStack = &temp_stack;

	// Initialize print manager
	DebugPrintManager print_manager;
	gPrintManager = &print_manager;

	// Create context
	CreateContext();

	{
#ifdef DEBUG
		// Open GL Debugging
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(OpenGLDebugCallback, nullptr);

		// Ignore some debug messages
		struct GLDebugIgnore
		{
			GLenum source;
			GLenum type;
			GLuint id;
		};

		GLDebugIgnore ignore[] = {
			{ GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, 131185 }, // GL_STATIC_DRAW info
		};

		for (const GLDebugIgnore& i : ignore)
		{
			glDebugMessageControl(
				i.source, i.type, GL_DONT_CARE,
				1, &i.id,
				false
			);
		}
#endif
	}

	// Resource manager
	ResourceManager resourceManager("res");
	gResourceManager = &resourceManager;

	// Debug draw
	DebugDrawManager debugDrawManager;
	gDebugDrawManager = &debugDrawManager;

	Scene scene;

	while (gContext->isOpen)
	{
		temp_stack.Reset();
		ContextUpdateFrame();

		// Get out if the window was closed this frame
		if (!gContext->isOpen || gContext->input.GetKeyPressed(Key::Escape))
			break;

#if DEBUG
		// Resource hotreloading
		static float HOTRELOAD_TIMER = 0.f;
		HOTRELOAD_TIMER += Time::Delta();
		if (HOTRELOAD_TIMER > 1.f)
		{
			resourceManager.UpdateHotReloading();
			HOTRELOAD_TIMER = 0.f;
		}
#endif

		// Main stuff!
		scene.Tick();

		// Swap
		ContextSwapBuffer();
	}

	DestroyContext();
	return 0;
}
#endif