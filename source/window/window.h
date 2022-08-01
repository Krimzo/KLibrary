#pragma once

#include <string>
#include <functional>
#include <windows.h>
#include <windowsx.h>

#include "graphics/image.h"
#include "window/keys.h"
#include "window/mouse.h"
#include "math/math.h"


namespace kl {
	namespace screen {
		inline const kl::uint2 size = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}

	class window {
		HINSTANCE m_Instance = nullptr;
		std::string m_Name = {};
		HWND m_Window = nullptr;
		HDC m_DeviceContext = nullptr;

		bool m_Fullscreened = false;
		bool m_Resizeable = true;
		DWORD m_WindowStyle = NULL;
		WINDOWPLACEMENT m_Placement = {};

		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleMessage(const MSG& message);

	public:
		kl::keys keys;
		kl::mouse mouse;

		std::function<void(kl::uint2)> resize = [](kl::uint2) {};

		window(const kl::uint2& size, const std::string& name);
		window(const kl::window&) = delete;
		void operator=(const kl::window&) = delete;
		~window();

		operator HWND() const;
		operator bool() const;

		bool process(bool wait = true);

		bool running() const;
		void close() const;

		bool resizeable() const;
		void resizeable(bool enabled);

		void maximize();
		void minimize();

		bool fullscreen() const;
		void fullscreen(bool enable);

		kl::int2 position(bool client = false) const;
		void position(const kl::int2& position);

		kl::uint2 size(bool client = true) const;
		void size(const kl::uint2& size, bool client = true);

		float aspect() const;
		kl::uint2 center() const;

		void title(const std::string& data);
		bool icon(const std::string& filePath);

		void draw(const kl::color* data, const kl::uint2& size, const kl::int2& position = {});
		void draw(const kl::image& toDraw, const kl::int2& position = {});

		void notify() const;
	};
}
