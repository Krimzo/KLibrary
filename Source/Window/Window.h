#pragma once

#include <string>
#include <functional>
#include <windows.h>
#include <windowsx.h>

#include "Graphics/Image.h"
#include "Window/Keyboard.h"
#include "Window/Mouse.h"
#include "Math/Math.h"


namespace kl {
	namespace Screen {
		inline const UInt2 Size = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}

	class Window {
		HINSTANCE m_Instance = nullptr;
		String m_Name = {};
		HWND m_Window = nullptr;
		HDC m_DeviceContext = nullptr;

		bool m_Fullscreened = false;
		bool m_Resizeable = true;
		DWORD m_WindowStyle = NULL;
		WINDOWPLACEMENT m_Placement = {};

		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleMessage(const MSG& message);

	public:
		Keyboard keyboard;
		Mouse mouse;

		Function<void(UInt2)> onResize = [](UInt2) {};

		Window(const UInt2& size, const String& name);
		Window(const Window&) = delete;
		void operator=(const Window&) = delete;
		~Window();

		operator HWND() const;
		operator bool() const;

		bool process(bool wait = true);

		bool isOpen() const;
		void close() const;

		bool isResizeable() const;
		void setResizeable(bool enabled);

		void maximize();
		void minimize();

		bool isFullscreened() const;
		void setFullscreen(bool enable);

		Int2 getPosition(bool client = false) const;
		void setPosition(const Int2& position);

		uint getWidth() const;
		void setWidth(uint width);

		uint getHeight() const;
		void setHeight(uint height);

		UInt2 getSize(bool client = true) const;
		void setSize(const UInt2& size, bool client = true);

		float getAspect() const;
		UInt2 getCenter() const;

		void setTitle(const String& data);
		bool setIcon(const String& filePath);

		void drawData(const Color* data, const UInt2& size, const Int2& position = {});
		void drawImage(const Image& image, const Int2& position = {});

		void notify() const;
	};
}
