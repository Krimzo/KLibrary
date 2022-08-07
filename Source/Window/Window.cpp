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


kl::Window::Window(const UInt2& size, const String& name) : m_Name(name) {
	// Instance
	m_Instance = GetModuleHandleA(nullptr);

	// Registering window class
	WNDCLASSEXA windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return ((Window*)GetWindowLongPtrA(hwnd, GWLP_USERDATA))->WndProc(hwnd, msg, wParam, lParam);
	};
	windowClass.hInstance = m_Instance;
	windowClass.lpszClassName = name.c_str();
	Assert(!RegisterClassExA(&windowClass), "Failed to register window class");

	// Creating the window
	m_WindowStyle = WS_OVERLAPPEDWINDOW;

	RECT sizeBuffer = { 0, 0, LONG(size.x), LONG(size.y) };
	AdjustWindowRect(&sizeBuffer, m_WindowStyle, false);

	UInt2 newSize = { sizeBuffer.right - sizeBuffer.left, sizeBuffer.bottom - sizeBuffer.top };
	Int2 newPosition = (Screen::Size / 2) - (newSize / 2);

	m_Window = CreateWindowExA(NULL, name.c_str(), name.c_str(), m_WindowStyle, newPosition.x, newPosition.y, newSize.x, newSize.y, nullptr, nullptr, m_Instance, nullptr);
	Assert(!m_Window, "Failed to create window");

	// Getting data
	m_DeviceContext = GetDC(m_Window);
	m_WindowStyle = GetWindowLongA(m_Window, GWL_STYLE);

	// Setting data
	SetWindowLongPtrA(m_Window, GWLP_USERDATA, int64(this));
	ShowWindow(m_Window, SW_SHOW);
	SetCursor(LoadCursorA(nullptr, LPCSTR(IDC_ARROW)));
	mouse.bindToWindow(m_Window);
}

kl::Window::~Window() {
	// Clearing DC
	ReleaseDC(m_Window, m_DeviceContext);
	DeleteDC(m_DeviceContext);

	// Clearing window
	DestroyWindow(m_Window);
	UnregisterClassA(m_Name.c_str(), m_Instance);
}

kl::Window::operator HWND() const {
	return m_Window;
}

kl::Window::operator bool() const {
	return isOpen();
}

bool kl::Window::process(bool wait) {
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

	keyboard.updateCall();
	mouse.updateCall();

	return isOpen();
}

bool kl::Window::isOpen() const {
	return IsWindow(m_Window);
}

void kl::Window::close() const {
	PostMessageA(m_Window, WM_CLOSE, NULL, NULL);
}

bool kl::Window::isResizeable() const {
	if (!m_Fullscreened) {
		return m_Resizeable;
	}
	return false;
}

void kl::Window::setResizeable(bool enabled) {
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

void kl::Window::maximize() {
	ShowWindow(m_Window, SW_MAXIMIZE);
}

void kl::Window::minimize() {
	ShowWindow(m_Window, SW_MINIMIZE);
}

bool kl::Window::isFullscreened() const {
	return m_Fullscreened;
}

void kl::Window::setFullscreen(bool enable) {
	if (!m_Fullscreened && enable) {
		GetWindowPlacement(m_Window, &m_Placement);
		SetWindowLongA(m_Window, GWL_STYLE, m_WindowStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_Window, HWND_TOP, 0, 0, Screen::Size.x, Screen::Size.y, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	else if (m_Fullscreened && !enable) {
		SetWindowLongA(m_Window, GWL_STYLE, m_WindowStyle);
		SetWindowPlacement(m_Window, &m_Placement);
		SetWindowPos(m_Window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	m_Fullscreened = enable;
}

kl::Int2 kl::Window::getPosition(bool client) const {
	RECT rect = {};
	if (client) {
		GetClientRect(m_Window, &rect);
	}
	else {
		GetWindowRect(m_Window, &rect);
	}
	return { rect.left, rect.top };
}

void kl::Window::setPosition(const kl::Int2& position) {
	if (!m_Fullscreened) {
		const UInt2 size = getSize(false);
		MoveWindow(m_Window, position.x, position.y, size.x, size.y, false);
	}
}

kl::uint kl::Window::getWidth() const {
	return getSize().x;
}

void kl::Window::setWidth(uint width) {
	setSize({ width, getHeight() });
}

kl::uint kl::Window::getHeight() const {
	return getSize().y;
}

void kl::Window::setHeight(uint height) {
	setSize({ getWidth(), height });
}

kl::UInt2 kl::Window::getSize(bool client) const {
	RECT rect = {};
	if (client) {
		GetClientRect(m_Window, &rect);
	}
	else {
		GetWindowRect(m_Window, &rect);
	}
	return { rect.right - rect.left, rect.bottom - rect.top };
}

void kl::Window::setSize(const kl::UInt2& size, bool client) {
	if (!m_Fullscreened) {
		const Int2 position = getPosition();
		UInt2 newSize = size;

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

float kl::Window::getAspect() const {
	const UInt2 winSize = getSize();
	return float(winSize.x) / winSize.y;
}

kl::UInt2 kl::Window::getCenter() const {
	return getSize() / 2;
}

void kl::Window::setTitle(const String& data) {
	SetWindowTextA(m_Window, data.c_str());
}

bool kl::Window::setIcon(const String& filePath) {
	HICON loadedIcon = ExtractIconA(nullptr, filePath.c_str(), NULL);
	if (!loadedIcon) {
		return false;
	}
	SendMessageA(m_Window, WM_SETICON, ICON_BIG, (LPARAM)loadedIcon);
	SendMessageA(m_Window, WM_SETICON, ICON_SMALL, (LPARAM)loadedIcon);
	return true;
}

void kl::Window::drawData(const Color* data, const UInt2& size, const Int2& position) {
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biWidth = size.x;
	bmpInfo.bmiHeader.biHeight = -int(size.y);

	StretchDIBits(m_DeviceContext, position.x, position.y, size.x, size.y, 0, 0, size.x, size.y, data, &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
}

void kl::Window::drawImage(const Image& image, const Int2& position) {
	drawData(image.data(), image.getSize(), position);
}

void kl::Window::notify() const {
	PostMessageA(m_Window, WM_NULL, NULL, NULL);
	PostMessageA(m_Window, WM_NULL, NULL, NULL);
}
