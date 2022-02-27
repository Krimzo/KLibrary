#pragma once

#include <string>
#include <functional>
#include <windows.h>
#include <windowsx.h>

#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/graphics/image.h"
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"

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
		HINSTANCE hInstance;
		HWND hwnd;
		HDC hdc;
		MSG wndMsg;

		// Fullscreen data
		bool inFull;
		DWORD winStyle;
		WINDOWPLACEMENT winPlace;

		// Registers a new window class
		void registerWindowClass(const std::string& name);

		// Creates a new window
		void createWindow(const kl::ivec2& size, const std::string& name, bool resizeable);

		// Handles the windows message
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void handleMessage();

		// ImGui
		bool usingImGui = false;

	public:
		// Screen
		class screen {
		public:
			screen() = delete;
			static const int width;
			static const int height;
		};

		// Input
		kl::keys keys;
		kl::mouse mouse;

		// User functions
		std::function<void(void)> start;
		std::function<void(void)> update;
		std::function<void(void)> end;
		std::function<void(const kl::ivec2& size)> onResize;

		// Constructor
		window();

		// Destructor
		~window();

		// Window creation
		void startNew(const kl::ivec2& size, const std::string& name, bool resizeable, bool continuous, bool imgui = false);

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
		kl::ivec2 getSize() const;

		// Returns the aspect ratio
		float getAspect() const;

		// Returns the center point of the frame
		kl::ivec2 getCenter() const;

		// Sets the window title
		void setTitle(const std::string& data);

		// Sets the window icons
		void setIcon(const std::string& filePath);

		// Sets the pixels of the window
		void drawImage(const kl::image& toDraw, const kl::ivec2& position = { 0, 0 });
	};
}
