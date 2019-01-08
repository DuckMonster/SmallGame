#include "context.h"
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include "Core/Time/PerformanceClock.h"

struct ContextData
{
	HWND hWnd;
	HINSTANCE hInstance;
	HDC hDc;
	HGLRC hGlContext;
};

struct WinKeyParams
{
	uint32 repeat : 16;
	uint32 scancode : 8;
	uint32 extended : 1;
	uint32 reserved : 4;
	uint32 context : 1;
	uint32 previous : 1;
	uint32 transition : 1;
};

struct WinSizeParams
{
	uint32 width : 16;
	uint32 height : 16;
};

struct WinMouseParams
{
	uint32 x : 16;
	uint32 y : 16;
};

struct WinMouseButtonParams
{
	uint8 Left : 1;
	uint8 Right : 1;
	uint8 Shift : 1;
	uint8 Control : 1;
	uint8 Middle : 1;
	uint8 X1 : 1;
	uint8 X2 : 1;
};

Context* gContext = nullptr;

namespace
{
	void InitOpenGLExtensions()
	{
		// Only load once
		static bool wasExtensionsLoaded = false;
		if (wasExtensionsLoaded)
			return;

		/* To initialize openGL properly, we have to first open up a dummy window and context
			The reason is that you can only set the pixel format for a window once */
		WNDCLASS dummyClass = { 0 };
		dummyClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		dummyClass.lpfnWndProc = DefWindowProc;
		dummyClass.hInstance = GetModuleHandle(0);
		dummyClass.lpszClassName = "Dummy_OpenGL";

		Assert(RegisterClass(&dummyClass));

		HWND dummyWindow = CreateWindowEx(
			0,
			dummyClass.lpszClassName,
			"Dummy OpenGL Window",
			0,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			dummyClass.hInstance,
			0
		);

		HDC dummyDC = GetDC(dummyWindow);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

		int pixelFormat = ChoosePixelFormat(dummyDC, &pfd);
		Assert(pixelFormat);
		Assert(SetPixelFormat(dummyDC, pixelFormat, &pfd));

		HGLRC dummyContext = wglCreateContext(dummyDC);
		wglMakeCurrent(dummyDC, dummyContext);

		// Init GLEW now!
		glewInit();

		// Clean up
		wglMakeCurrent(dummyDC, 0);
		wglDeleteContext(dummyContext);
		ReleaseDC(dummyWindow, dummyDC);
		DestroyWindow(dummyWindow);

		wasExtensionsLoaded = true;
	}

	HGLRC CreateOpenGLContext(HDC deviceContext)
	{
		// Init extensions first for 3.3 goodness
		InitOpenGLExtensions();

		// Fetch pixel format
		int pixelFormatAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB,		GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,		GL_TRUE,
			WGL_ACCELERATION_ARB,		WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,			WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,			32,
			WGL_DEPTH_BITS_ARB,			24,
			WGL_STENCIL_BITS_ARB,		8,
			0
		};

		int pixelFormat;
		UINT numFormats;

		wglChoosePixelFormatARB(deviceContext, pixelFormatAttribs, 0, 1, &pixelFormat, &numFormats);
		Assert(numFormats);

		// Set it
		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(deviceContext, pixelFormat, sizeof(pfd), &pfd);
		Assert(SetPixelFormat(deviceContext, pixelFormat, &pfd));

		// Initialize 3.3 context
		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
			WGL_CONTEXT_MINOR_VERSION_ARB,	3,
			WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		HGLRC glContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
		Assert(glContext);

		wglMakeCurrent(deviceContext, glContext);
		return glContext;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	// -- CREATE --
	case WM_CREATE:
	{
		// Create GL context
		HDC deviceContext = GetDC(hWnd);
		HGLRC glContext = CreateOpenGLContext(deviceContext);

		// VSYNC off bro
		wglSwapIntervalEXT(0);

		// Set context data now that we have it
		gContext->data->hWnd = hWnd;
		gContext->data->hDc = deviceContext;
		gContext->data->hGlContext = glContext;
		break;
	}

	// -- ACTIVATE (focus) --
	case WM_ACTIVATE:
	{
		gContext->isFocused = wParam > 0;
		break;
	}

	// -- KEY DOWN --
	case WM_KEYDOWN:
	{
		if (!gContext->isFocused)
			break;

		WinKeyParams* key = (WinKeyParams*)&lParam;

		// Ignore key repeats
		if (key->previous)
			break;

		if (key->scancode >= (uint16)Key::MAX)
		{
			Error("Invalid scancode %x (%d)", key->scancode, key->scancode);
			break;
		}

		gContext->input.SetKey((Key)key->scancode, true);
		break;
	}

	// -- KEY UP --
	case WM_KEYUP:
	{
		if (!gContext->isFocused)
			break;

		WinKeyParams* key = (WinKeyParams*)&lParam;

		if (key->scancode >= (uint16)Key::MAX)
		{
			Error("Invalid scancode %d", key->scancode);
			break;
		}

		gContext->input.SetKey((Key)key->scancode, false);
		break;
	}

	// -- MOUSE MOVE --
	case WM_MOUSEMOVE:
	{
		if (!gContext->isFocused)
			break;

		WinMouseParams* mouse = (WinMouseParams*)&lParam;
		gContext->input.mouseState.x = mouse->x;
		gContext->input.mouseState.y = mouse->y;
		break;
	}

	// -- MOUSE INPUTS --
	case WM_LBUTTONDOWN:
	{
		if (!gContext->isFocused)
			break;

		gContext->input.SetMouseButton(MouseButton::Left, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (!gContext->isFocused)
			break;

		gContext->input.SetMouseButton(MouseButton::Left, false);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (!gContext->isFocused)
			break;

		gContext->input.SetMouseButton(MouseButton::Right, true);
		break;
	}
	case WM_RBUTTONUP:
	{
		if (!gContext->isFocused)
			break;

		gContext->input.SetMouseButton(MouseButton::Right, false);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (!gContext->isFocused)
			break;

		gContext->input.SetMouseButton(MouseButton::Middle, true);
		break;
	}
	case WM_MBUTTONUP:
	{
		if (!gContext->isFocused)
			break;

		gContext->input.SetMouseButton(MouseButton::Middle, false);
		break;
	}
	case WM_XBUTTONDOWN:
	{
		if (!gContext->isFocused)
			break;

		// Find out which XButton by looking at the high-order of wParam
		uint16 highByte = ((uint16*)&wParam)[1];
		if ((highByte & 3) == 0)
		{
			Error("Unknown XButton was pressed");
			break;
		}

		MouseButton btn = highByte == 1 ? MouseButton::X1 : MouseButton::X2;
		gContext->input.SetMouseButton(btn, true);
		break;
	}
	case WM_XBUTTONUP:
	{
		if (!gContext->isFocused)
			break;

		// Find out which XButton by looking at the high-order of wParam
		uint16 highByte = ((uint16*)&wParam)[1];
		if ((highByte & 3) == 0)
		{
			Error("Unknown XButton was pressed");
			break;
		}

		MouseButton btn = highByte == 1 ? MouseButton::X1 : MouseButton::X2;
		gContext->input.SetMouseButton(btn, false);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		// The wheel delta is the high-order word of wParam
		float delta = ((int16*)&wParam)[1];

		// The delta is in order of this WHEEL_DELTA constant
		delta /= WHEEL_DELTA;
		gContext->input.mouseState.wheel += delta;
		break;
	}

	// -- RESIZE --
	case WM_SIZE:
	{
		WinSizeParams* size = (WinSizeParams*)&lParam;
		gContext->width = size->width;
		gContext->height = size->height;

		glViewport(0, 0, gContext->width, gContext->height);

		break;
	}

	// -- DESTROY --
	case WM_DESTROY:
	{
		wglDeleteContext(gContext->data->hGlContext);
		delete gContext->data;
		gContext->data = nullptr;
		gContext->isOpen = false;

		break;
	}
	}

	return DefWindowProc(hWnd, Message, wParam, lParam);
}

bool CreateContext()
{
	static bool classWasRegistered = false;
	static LPCSTR className = "WindowClass";

	Assert(gContext == nullptr);

	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Create and register window class
	if (!classWasRegistered)
	{
		WNDCLASS wc = { 0 };
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
		wc.lpszClassName = className;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_OWNDC;

		if (!RegisterClass(&wc))
			return false;

		classWasRegistered = true;
	}

	// Create the global context data
	gContext = new Context();
	gContext->width = 640;
	gContext->height = 480;
	gContext->isOpen = true;
	gContext->data = new ContextData;
	gContext->data->hInstance = hInstance;

	// Open window
	CreateWindow(className, "Hello World!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1600, 1024, 0, 0, hInstance, 0);

	PerformanceClock::Init();
	Time::Init();
	return true;
}

void ContextUpdateFrame()
{
	// Reset mouse wheel for this frame
	gContext->input.mouseState.prevWheel = gContext->input.mouseState.wheel;
	gContext->input.inputFrame++;
	Sleep(1);

	MSG msg = { 0 };
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Time::Update();
	Time::BeginFrame();
}

void ContextSwapBuffer()
{
	Time::EndFrame();
	Assert(gContext != nullptr);

	// Swap dat
	SwapBuffers(gContext->data->hDc);
}

void DestroyContext()
{
	if (gContext == nullptr)
		return;

	if (gContext->data != nullptr)
	{
		// The context is still open, so manually clean up here
		DestroyWindow(gContext->data->hWnd);
	}

	delete gContext;
	gContext = nullptr;
}
