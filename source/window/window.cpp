#include "window/window.h"

#include "utility/convert.h"
#include "utility/console.h"

#ifdef KL_USING_IMGUI
#include "imgui.h"
#include "imgui_impl_win32.h"
#endif


// Screen
const kl::int2 kl::window::screen = kl::int2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

// Constr/destr
kl::window::window() {}
kl::window::~window() {
	stop();
}

// Registers a new window class
void kl::window::registerWindowClass(const String& name) {
	WNDCLASSEXA windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		kl::window* callingWin = (kl::window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		return callingWin->WndProc(hwnd, msg, wParam, lParam);
	};
	windowClass.hInstance = inst;
	windowClass.lpszClassName = name.c_str();
	if (!RegisterClassExA(&windowClass)) {
		kl::console::show();
		std::cout << "WinApi: Could not register a window class!";
		std::cin.get();
		exit(69);
	}
}

// Creates a new window
void kl::window::createWindow(const kl::int2& size, const String& name, bool resizeable) {
	// Setting the window properties
	winStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	RECT adjustedWindowSize = { 0, 0, LONG(size.x), LONG(size.y) };
	AdjustWindowRect(&adjustedWindowSize, winStyle, false);
	const kl::int2 adjSize(adjustedWindowSize.right - adjustedWindowSize.left, adjustedWindowSize.bottom - adjustedWindowSize.top);

	// Creating the window
	wnd = CreateWindowExA(0, name.c_str(), name.c_str(), winStyle, (kl::window::screen.x / 2 - adjSize.x / 2), (kl::window::screen.y / 2 - adjSize.y / 2), adjSize.x, adjSize.y, nullptr, nullptr, inst, nullptr);
	if (!wnd) {
		kl::console::show();
		std::cout << "WinApi: Could not create a window!";
		std::cin.get();
		exit(69);
	}
	SetWindowLongPtrA(wnd, GWLP_USERDATA, (long long)this);

	// Setting and getting window info
	ShowWindow(wnd, SW_SHOW);
	dc = GetDC(wnd);

	// Saving a complete window style
	winStyle = GetWindowLongA(wnd, GWL_STYLE);
}

// Handles the windows message
#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_SIZE:
		resize(kl::int2(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}
void kl::window::handleMessage(MSG& msg) {
#ifdef KL_USING_IMGUI
	TranslateMessage(&msg);
	if (ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam)) {
		return;
	}
#endif

	// Default
	switch (msg.message) {
#ifdef KL_USING_IMGUI
	case WM_CHAR:
		if (*(short*)&msg.lParam > 1) {
			ImGui::GetIO().AddInputCharacter(uint32_t(msg.wParam));
		}
		break;
#endif

	case WM_KEYDOWN:
		keys.updateKey(msg.wParam, true);
		break;

	case WM_KEYUP:
		keys.updateKey(msg.wParam, false);
		break;

	case WM_LBUTTONDOWN:
		mouse.lmb.update(true);
		break;

	case WM_LBUTTONUP:
		mouse.lmb.update(false);
		break;

	case WM_MBUTTONDOWN:
		mouse.mmb.update(true);
		break;

	case WM_MBUTTONUP:
		mouse.mmb.update(false);
		break;

	case WM_RBUTTONDOWN:
		mouse.rmb.update(true);
		break;

	case WM_RBUTTONUP:
		mouse.rmb.update(false);
		break;

	case WM_MOUSEMOVE:
		mouse.position = kl::int2(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
		break;

	case WM_MOUSEWHEEL:
		mouse.scroll += GET_WHEEL_DELTA_WPARAM(msg.wParam) / 120;
		break;

	default:
		DispatchMessageA(&msg);
		break;
	}
}

// Window creation
void kl::window::startNew(const kl::int2& size, const String& name, bool resizeable, bool continuous) {
	// Registering winapi window class
	registerWindowClass(name);

	// Creating a window
	createWindow(size, name, resizeable);

	// Binding the mouse
	mouse.bind(wnd);

#ifdef KL_USING_IMGUI
	ImGui_ImplWin32_Init(wnd);
#endif

	// Starting the update loops
	MSG msg = {};
	SetCursor(LoadCursorA(nullptr, LPCSTR(IDC_ARROW)));
	if (continuous) {
		start();
		while (IsWindow(wnd)) {
			while (PeekMessageA(&msg, wnd, 0, 0, PM_REMOVE)) {
				handleMessage(msg);
			}
			keys.callAllDowns();
			mouse.callAllDowns();
			update();
		}
		end();
	}
	else {
		start();
		while (IsWindow(wnd)) {
			GetMessageA(&msg, wnd, 0, 0);
			handleMessage(msg);
			keys.callAllDowns();
			mouse.callAllDowns();
			update();
		}
		end();
	}

	// Cleanup
#ifdef KL_USING_IMGUI
	ImGui_ImplWin32_Shutdown();
#endif
	UnregisterClassA(name.c_str(), inst);
	wnd = nullptr;
	dc = nullptr;
	msg = {};
	inFull = false;
	winStyle = NULL;
	winPlace = {};
}
void kl::window::stop() const {
	PostMessageA(wnd, WM_CLOSE, 0, 0);
}

bool kl::window::isOpen() const {
	return bool(wnd);
}

// Returns a handle to the window
HWND kl::window::getWND() {
	return wnd;
}

// Sets the fullscreen mode
void kl::window::setFullscreen(bool enable) {
	if (!inFull && enable) {
		// Saving old window position
		GetWindowPlacement(wnd, &winPlace);

		// Enabling the fullscreen
		SetWindowLongA(wnd, GWL_STYLE, winStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(wnd, HWND_TOP, 0, 0, kl::window::screen.x, kl::window::screen.y, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		// Setting info
		inFull = true;
	}
	else if (inFull && !enable) {
		// Resetting the size
		SetWindowLongA(wnd, GWL_STYLE, winStyle);
		SetWindowPlacement(wnd, &winPlace);
		SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		// Setting info
		inFull = false;
	}
}

// Max/min functions
void kl::window::maximize() {
	ShowWindow(wnd, SW_MAXIMIZE);
}
void kl::window::minimize() {
	ShowWindow(wnd, SW_MINIMIZE);
}

// Returns the window size
kl::int2 kl::window::getSize() const {
	RECT clientArea = {};
	GetClientRect(wnd, &clientArea);
	return kl::int2(clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
}

// Returns the aspect ratio
float kl::window::getAspect() const {
	const kl::int2 winSize = getSize();
	return float(winSize.x) / winSize.y;
}

// Returns the center point of the frame
kl::int2 kl::window::getCenter() const {
	return getSize() / 2;
}

// Sets the window title
void kl::window::setTitle(const String& data) {
	SetWindowTextA(wnd, data.c_str());
}

// Sets the window icons
bool kl::window::setIcon(const String& filePath) {
	// Loading the icon
	HICON loadedIcon = ExtractIconA(nullptr, filePath.c_str(), NULL);
	if (!loadedIcon) {
		kl::console::show();
		std::cout << "WinApi: Could not load an icon file \"" << filePath << "\"";
		return false;
	}

	// Sending the icon
	SendMessageA(wnd, WM_SETICON, ICON_BIG, (LPARAM)loadedIcon);
	SendMessageA(wnd, WM_SETICON, ICON_SMALL, (LPARAM)loadedIcon);
	return true;
}

// Sets the pixels of the window
void kl::window::drawImage(const kl::image& toDraw, const kl::int2& position) {
	// Getting the image size
	const kl::int2 size = toDraw.size();

	// Setting up the bitmapinfo
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biWidth = size.x;
	bmpInfo.bmiHeader.biHeight = -size.y;

	// Drawing
	StretchDIBits(dc, position.x, position.y, size.x, size.y, 0, 0, size.x, size.y, toDraw.pointer(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
}
