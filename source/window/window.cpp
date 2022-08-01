#include "window/window.h"
#include "utility/console.h"

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


kl::window::window(const kl::uint2& size, const std::string& name) : m_Name(name) {
	// Instance
	m_Instance = GetModuleHandleA(nullptr);

	// Registering window class
	WNDCLASSEXA windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return ((kl::window*) GetWindowLongPtrA(hwnd, GWLP_USERDATA))->WndProc(hwnd, msg, wParam, lParam);
	};
	windowClass.hInstance = m_Instance;
	windowClass.lpszClassName = name.c_str();
	kl::console::error(!RegisterClassExA(&windowClass), "Failed to register window class");

	// Creating the window
	m_WindowStyle = WS_OVERLAPPEDWINDOW;

	RECT sizeBuffer = { 0, 0, LONG(size.x), LONG(size.y) };
	AdjustWindowRect(&sizeBuffer, m_WindowStyle, false);

	kl::uint2 newSize = { sizeBuffer.right - sizeBuffer.left, sizeBuffer.bottom - sizeBuffer.top };
	kl::int2 newPosition = (kl::screen::size / 2) - (newSize / 2);

	m_Window = CreateWindowExA(NULL, name.c_str(), name.c_str(), m_WindowStyle,
		newPosition.x, newPosition.y, newSize.x, newSize.y, nullptr, nullptr, m_Instance, nullptr);
	kl::console::error(!m_Window, "Failed to create window");

	// Getting data
	m_DeviceContext = GetDC(m_Window);
	m_WindowStyle = GetWindowLongA(m_Window, GWL_STYLE);

	// Setting data
	SetWindowLongPtrA(m_Window, GWLP_USERDATA, int64(this));
	ShowWindow(m_Window, SW_SHOW);
	SetCursor(LoadCursorA(nullptr, LPCSTR(IDC_ARROW)));
	mouse.bind(m_Window);
}

kl::window::~window() {
	// Clearing DC
	ReleaseDC(m_Window, m_DeviceContext);
	DeleteDC(m_DeviceContext);

	// Clearing window
	DestroyWindow(m_Window);
	UnregisterClassA(m_Name.c_str(), m_Instance);
}

kl::window::operator HWND() const {
	return m_Window;
}

kl::window::operator bool() const {
	return running();
}

bool kl::window::process(bool wait) {
	MSG message;
	if (wait) {
		GetMessageA(&message, m_Window, 0, 0);
		HandleMessage(message);
	}
	else {
		while (PeekMessageA(&message, m_Window, 0, 0, PM_REMOVE)) {
			HandleMessage(message);
		}
	}
	keys.update();
	mouse.update();
	return running();
}

bool kl::window::running() const {
	return IsWindow(m_Window);
}

void kl::window::close() const {
	PostMessageA(m_Window, WM_CLOSE, NULL, NULL);
}

bool kl::window::resizeable() const {
	if (!m_Fullscreened) {
		return m_Resizeable;
	}
	return false;
}

void kl::window::resizeable(bool enabled) {
	if (!m_Fullscreened) {
		if (!m_Resizeable && enabled) {
			SetWindowLongA(m_Window, GWL_STYLE, GetWindowLongA(m_Window, GWL_STYLE) | WS_SIZEBOX | WS_MAXIMIZEBOX);
			m_WindowStyle = GetWindowLongA(m_Window, GWL_STYLE);
		}
		else if (m_Resizeable && !enabled) {
			SetWindowLongA(m_Window, GWL_STYLE, GetWindowLongA(m_Window, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
			m_WindowStyle = GetWindowLongA(m_Window, GWL_STYLE);
		}
		m_Resizeable = enabled;
	}
}

void kl::window::maximize() {
	ShowWindow(m_Window, SW_MAXIMIZE);
}

void kl::window::minimize() {
	ShowWindow(m_Window, SW_MINIMIZE);
}

bool kl::window::fullscreen() const {
	return m_Fullscreened;
}

void kl::window::fullscreen(bool enable) {
	if (!m_Fullscreened && enable) {
		GetWindowPlacement(m_Window, &m_Placement);
		SetWindowLongA(m_Window, GWL_STYLE, m_WindowStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_Window, HWND_TOP, 0, 0, kl::screen::size.x, kl::screen::size.y, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	else if (m_Fullscreened && !enable) {
		SetWindowLongA(m_Window, GWL_STYLE, m_WindowStyle);
		SetWindowPlacement(m_Window, &m_Placement);
		SetWindowPos(m_Window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	m_Fullscreened = enable;
}

kl::int2 kl::window::position(bool client) const {
	RECT rect = {};
	if (client) {
		GetClientRect(m_Window, &rect);
	}
	else {
		GetWindowRect(m_Window, &rect);
	}
	return { rect.left, rect.top };
}

void kl::window::position(const kl::int2& position) {
	if (!m_Fullscreened) {
		kl::uint2 size = this->size(false);
		MoveWindow(m_Window, position.x, position.y, size.x, size.y, false);
	}
}

kl::uint2 kl::window::size(bool client) const {
	RECT rect = {};
	if (client) {
		GetClientRect(m_Window, &rect);
	}
	else {
		GetWindowRect(m_Window, &rect);
	}
	return { rect.right - rect.left, rect.bottom - rect.top };
}

void kl::window::size(const kl::uint2& size, bool client) {
	if (!m_Fullscreened) {
		kl::int2 position = this->position();
		kl::uint2 newSize = size;

		if (client) {
			RECT rect = {
				LONG(position.x),
				LONG(position.y),
				LONG(position.x + size.x),
				LONG(position.y + size.y)
			};
			AdjustWindowRect(&rect, m_WindowStyle, false);
			newSize = { rect.right - rect.left, rect.bottom - rect.top };
		}

		MoveWindow(m_Window, position.x, position.y, newSize.x, newSize.y, false);
	}
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
