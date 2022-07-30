#include "window/window.h"


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


kl::window::window() {}
kl::window::~window() {
	stop();
}

void kl::window::run(const kl::uint2& size, const std::string& name, bool resizeable, bool continuous) {
	registerWindowClass(name);
	createWindow(size, name, resizeable);
	mouse.bind(m_Window);

	MSG msg = {};
	SetCursor(LoadCursorA(nullptr, LPCSTR(IDC_ARROW)));
	if (continuous) {
		start();
		while (IsWindow(m_Window)) {
			while (PeekMessageA(&msg, m_Window, 0, 0, PM_REMOVE)) {
				handleMessage(msg);
			}
			keys.update();
			mouse.update();
			update();
		}
		end();
	}
	else {
		start();
		while (IsWindow(m_Window)) {
			GetMessageA(&msg, m_Window, 0, 0);
			handleMessage(msg);
			keys.update();
			mouse.update();
			update();
		}
		end();
	}

	UnregisterClassA(name.c_str(), m_Instance);
	m_Window = nullptr;
	m_DeviceContext = nullptr;
	m_Fullscreened = false;
	m_WindowStyle = NULL;
	m_Placement = {};
}
void kl::window::stop() const {
	PostMessageA(m_Window, WM_CLOSE, NULL, NULL);
}
bool kl::window::running() const {
	return bool(m_Window);
}

kl::window::operator HWND() const {
	return m_Window;
}

void kl::window::fullscreen(bool enable) {
	if (!m_Fullscreened && enable) {
		GetWindowPlacement(m_Window, &m_Placement);
		SetWindowLongA(m_Window, GWL_STYLE, m_WindowStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_Window, HWND_TOP, 0, 0, kl::screen::size.x, kl::screen::size.y, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		m_Fullscreened = true;
	}
	else if (m_Fullscreened && !enable) {
		SetWindowLongA(m_Window, GWL_STYLE, m_WindowStyle);
		SetWindowPlacement(m_Window, &m_Placement);
		SetWindowPos(m_Window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		m_Fullscreened = false;
	}
}

void kl::window::maximize() {
	ShowWindow(m_Window, SW_MAXIMIZE);
}
void kl::window::minimize() {
	ShowWindow(m_Window, SW_MINIMIZE);
}

kl::uint2 kl::window::size() const {
	RECT clientArea = {};
	GetClientRect(m_Window, &clientArea);
	return kl::uint2(clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
}
float kl::window::aspect() const {
	const kl::uint2 winSize = size();
	return float(winSize.x) / winSize.y;
}
kl::uint2 kl::window::center() const {
	return size() / 2;
}

void kl::window::title(const std::string& data) {
	SetWindowTextA(m_Window, data.c_str());
}

bool kl::window::icon(const std::string& filePath) {
	HICON loadedIcon = ExtractIconA(nullptr, filePath.c_str(), NULL);
	if (!loadedIcon) {
		return false;
	}
	SendMessageA(m_Window, WM_SETICON, ICON_BIG, (LPARAM) loadedIcon);
	SendMessageA(m_Window, WM_SETICON, ICON_SMALL, (LPARAM) loadedIcon);
	return true;
}

void kl::window::draw(const kl::color* data, const kl::uint2& size, const kl::int2& position) {
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biWidth = size.x;
	bmpInfo.bmiHeader.biHeight = -int(size.y);
	StretchDIBits(m_DeviceContext, position.x, position.y, size.x, size.y, 0, 0, size.x, size.y, data, &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
}
void kl::window::draw(const kl::image& toDraw, const kl::int2& position) {
	draw(toDraw.data(), toDraw.size(), position);
}

void kl::window::notify() const {
	PostMessageA(m_Window, WM_NULL, NULL, NULL);
	PostMessageA(m_Window, WM_NULL, NULL, NULL);
}
