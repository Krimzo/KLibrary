#pragma once

#include <string>
#include <functional>
#include <windows.h>
#include <windowsx.h>

#include "libspec/using.h"
#include "math/int2.h"
#include "graphics/image.h"
#include "window/keys.h"
#include "window/mouse.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")


namespace kl {
	class window {
	private:
		// Winapi variables
		HINSTANCE inst = GetModuleHandleA(nullptr);
		HWND wnd = nullptr;
		HDC dc = nullptr;

		// Fullscreen data
		bool inFull = false;
		DWORD winStyle = NULL;
		WINDOWPLACEMENT winPlace = {};

		// Registers a new window class
		void registerWindowClass(const String& name);

		// Creates a new window
		void createWindow(const kl::int2& size, const String& name, bool resizeable);

		// Handles the windows message
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void handleMessage(MSG& msg);

	public:
		// Screen size
		static const kl::int2 screen;

		// Input
		kl::keys keys;
		kl::mouse mouse;

		// User functions
		std::function<void(void)> start = []() {};
		std::function<void(void)> update = []() {};
		std::function<void(void)> end = []() {};
		std::function<void(const kl::int2&)> resize = [](const kl::int2&) {};

		window();
		window(const kl::window&) = delete;
		~window();

		bool isOpen() const;

		// Window creation
		void startNew(const kl::int2& size, const String& name, bool resizeable, bool continuous);

		// Window stop
		void stop() const;

		// Returns a handle to the window
		HWND getWND();

		// Sets the fullscreen mode
		void setFullscreen(bool enable);

		// Max/min functions
		void maximize();
		void minimize();

		// Returns the window size
		kl::int2 getSize() const;

		// Returns the aspect ratio
		float getAspect() const;

		// Returns the center point of the frame
		kl::int2 getCenter() const;

		// Sets the window title
		void setTitle(const String& data);

		// Sets the window icons
		bool setIcon(const String& filePath);

		// Sets the pixels of the window
		void drawImage(const kl::image& toDraw, const kl::int2& position = { 0, 0 });
	};
}
