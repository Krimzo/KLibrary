#pragma once

#include <string>
#include <functional>
#include <windows.h>
#include <windowsx.h>

#include "KrimzLib/gl/gl.h"
#include "KrimzLib/vecmath/ivec2.h"
#include "KrimzLib/graphics/image.h"
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"


namespace kl {
	// Screen size
	namespace screen {
		
	}
	
	// Window class
	class window {
	public:
		// Screen
		static class screen {
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

		// Constructor
		window();

		// Destructor
		~window();

		// Window creation
		void startNew(const kl::ivec2& size, const std::string& name, bool resizeable = true, bool continuous = false, bool opengl = false);
		
		// Window stop
		void stop();

		// Returns a handle to the window
		HWND getHWND();

		// Sets the fullscreen mode
		void setFullscreen(bool enable);

		// Returns the window size
		kl::ivec2 getSize() const;

		// Returns the aspect ratio
		float getAspect() const;

		// Returns the center point of the frame
		kl::ivec2 getCenter() const;

		// Sets the window title
		void setTitle(const std::string& data);

		// Sets the pixels of the window
		void drawImage(const kl::image& toDraw, const kl::ivec2& position = { 0, 0 });

		// Binds the OpenGL contex of the window
		void makeCurrentGL();

		// Resets the OpenGL viewport
		void resetViewport();

		// Swaps the front and back buffers
		void swapBuffers();

	private:
		// Winapi variables
		HINSTANCE hInstance;
		HWND hwnd;
		HDC hdc;
		HGLRC hglrc;
		BITMAPINFO bmpInfo;
		MSG wndMsg;

		// Fullscreen data
		bool inFull;
		DWORD winStyle;
		WINDOWPLACEMENT winPlace;

		// Registers a new window class
		void registerWindowClass(const std::wstring& name);

		// Creates a new window
		void createWindow(const kl::ivec2& size, const std::wstring& name, bool resizeable);

		// Sets up the bitmap properties
		void setupBitmapInfo();

		// Sets up OpenGL context
		void initOpenGL();

		// Handles the windows message
		void handleMessage();

		// Destroys the contexts
		void cleanup(const std::wstring& name, bool opengl);
	};
}
