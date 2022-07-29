#pragma once

#include <string>
#include <functional>
#include <windows.h>
#include <windowsx.h>

#include "graphics/image.h"
#include "window/keys.h"
#include "window/mouse.h"
#include "math/math.h"


namespace kl
{
	namespace screen
	{
		inline kl::uint2 size = kl::int2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}

	class window
	{
	private:
		HINSTANCE m_Instance = GetModuleHandleA(nullptr);
		HWND m_Window = nullptr;
		HDC m_DeviceContext = nullptr;

		bool m_Fullscreened = false;
		DWORD m_WindowStyle = NULL;
		WINDOWPLACEMENT m_Placement = {};

		void registerWindowClass(const std::string& name);
		void createWindow(const kl::uint2& size, const std::string& name, bool resizeable);
		LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void handleMessage(MSG& msg);

	public:
		kl::keys keys;
		kl::mouse mouse;

		std::function<void(void)> start = []()
		{};
		std::function<void(void)> update = []()
		{};
		std::function<void(void)> end = []()
		{};
		std::function<void(const kl::uint2&)> resize = [](const kl::uint2&)
		{};

		window();
		window(const kl::window&) = delete;
		void operator=(const kl::window&) = delete;
		~window();

		void run(const kl::uint2& size, const std::string& name, bool resizeable = true, bool continuous = false);
		void stop() const;
		bool running() const;

		operator HWND() const;

		void maximize();
		void minimize();
		void fullscreen(bool enable);

		kl::uint2 size() const;
		float aspect() const;
		kl::uint2 center() const;

		void title(const std::string& data);
		bool icon(const std::string& filePath);

		void draw(const kl::color* data, const kl::uint2& size, const kl::int2& position = {});
		void draw(const kl::image& toDraw, const kl::int2& position = {});

		void notify() const;
	};
}
