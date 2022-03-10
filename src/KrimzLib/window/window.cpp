#include "KrimzLib/window/window.h"

#include "KrimzLib/utility/convert.h"

#ifdef KL_USING_IMGUI
#include "ImGui/imgui_impl_win32.h"
#endif


// Screen
const int kl::window::screen::width = GetSystemMetrics(SM_CXSCREEN);
const int kl::window::screen::height = GetSystemMetrics(SM_CYSCREEN);

// Constructor
kl::window::window() {
	// User methods
	this->start = []() {};
	this->update = []() {};
	this->end = []() {};
	this->resize = [](const kl::int2& size) {};

	// Winapi variables
	this->hInstance = GetModuleHandleA(nullptr);
	this->hwnd = nullptr;
	this->hdc = nullptr;
	this->wndMsg = {};

	// Fullscreen data
	this->inFull = false;
	this->winStyle = NULL;
	this->winPlace = {};
}

// Destructor
kl::window::~window() {
	this->stop();
}

// Registers a new window class
void kl::window::registerWindowClass(const std::string& name) {
	WNDCLASSEXA windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		kl::window* callingWin = (kl::window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		return callingWin->WndProc(hwnd, msg, wParam, lParam);
	};
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = nullptr;
	windowClass.hCursor = nullptr;
	windowClass.hbrBackground = nullptr;
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = name.c_str();
	windowClass.hIconSm = nullptr;
	if (!RegisterClassExA(&windowClass)) {
		std::cout << "WinApi: Could not register a window class!";
		std::cin.get();
		exit(69);
	}
}

// Creates a new window
void kl::window::createWindow(const kl::int2& size, const std::string& name, bool resizeable) {
	// Setting the window properties
	winStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	RECT adjustedWindowSize = { 0, 0, (LONG)size.x, (LONG)size.y };
	AdjustWindowRect(&adjustedWindowSize, winStyle, false);
	const kl::int2 adjSize(adjustedWindowSize.right - adjustedWindowSize.left, adjustedWindowSize.bottom - adjustedWindowSize.top);

	// Creating the window
	hwnd = CreateWindowExA(0, name.c_str(), name.c_str(), winStyle, (kl::window::screen::width / 2 - adjSize.x / 2), (kl::window::screen::height / 2 - adjSize.y / 2), adjSize.x, adjSize.y, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd) {
		std::cout << "WinApi: Could not create a window!";
		std::cin.get();
		exit(69);
	}
	SetWindowLongPtrA(hwnd, GWLP_USERDATA, (long long)this);

	// Setting and getting window info
	ShowWindow(hwnd, SW_SHOW);
	hdc = GetDC(hwnd);

	// Saving a complete window style
	winStyle = GetWindowLongA(hwnd, GWL_STYLE);
}

// Handles the windows message
#ifdef KL_USING_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK kl::window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_SIZE:
		this->resize(kl::int2(LOWORD(lParam), HIWORD(lParam)));
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lParam);
}
void kl::window::handleMessage() {
#ifdef KL_USING_IMGUI
	// ImGui
	if (usingImGui) {
		if (ImGui_ImplWin32_WndProcHandler(wndMsg.hwnd, wndMsg.message, wndMsg.wParam, wndMsg.lParam)) {
			return;
		}
	}
#endif

	// Default
	switch (wndMsg.message) {
	case WM_KEYDOWN:
		this->keys.setKey(wndMsg.wParam, true);
		break;

	case WM_KEYUP:
		this->keys.setKey(wndMsg.wParam, false);
		break;

	case WM_LBUTTONDOWN:
		this->mouse.lmb = true;
		break;

	case WM_LBUTTONUP:
		this->mouse.lmb = false;
		break;

	case WM_MBUTTONDOWN:
		this->mouse.mmb = true;
		break;

	case WM_MBUTTONUP:
		this->mouse.mmb = false;
		break;

	case WM_RBUTTONDOWN:
		this->mouse.rmb = true;
		break;

	case WM_RBUTTONUP:
		this->mouse.rmb = false;
		break;

	case WM_MOUSEMOVE:
		this->mouse.position = kl::int2(GET_X_LPARAM(wndMsg.lParam), GET_Y_LPARAM(wndMsg.lParam));
		break;

	case WM_MOUSEWHEEL:
		this->mouse.scroll += GET_WHEEL_DELTA_WPARAM(wndMsg.wParam) / 120;
		break;

	default:
		DispatchMessageA(&wndMsg);
		break;
	}
}

// Window creation
void kl::window::startNew(const kl::int2& size, const std::string& name, bool resizeable, bool continuous, bool imgui) {
	// Registering winapi window class
	registerWindowClass(name);

	// Creating a window
	createWindow(size, name, resizeable);

	// Binding the mouse
	this->mouse.bind(hwnd);

#ifdef KL_USING_IMGUI
	// ImGui setup
	this->usingImGui = imgui;
	if (imgui) {
		ImGui_ImplWin32_Init(hwnd);
	}
#endif

	// Starting the update loops
	SetCursor(LoadCursorA(nullptr, LPCSTR(IDC_ARROW)));
	if (continuous) {
		start();
		while (IsWindow(hwnd)) {
			while (PeekMessageA(&wndMsg, hwnd, 0, 0, PM_REMOVE)) {
				handleMessage();
			}
			update();
		}
		end();
	}
	else {
		start();
		while (IsWindow(hwnd)) {
			GetMessageA(&wndMsg, hwnd, 0, 0);
			handleMessage();
			update();
		}
		end();
	}

#ifdef KL_USING_IMGUI
	// ImGui cleanup
	if (imgui) {
		ImGui_ImplWin32_Shutdown();
	}
#endif

	// Cleanup
	UnregisterClassA(name.c_str(), hInstance);
}
void kl::window::stop() const {
	PostMessageA(hwnd, WM_CLOSE, 0, 0);
}

// Returns a handle to the window
HWND kl::window::getWND() {
	return hwnd;
}

// Sets the fullscreen mode
void kl::window::setFullscreen(bool enable) {
	if (!inFull && enable) {
		// Saving old window position
		GetWindowPlacement(hwnd, &winPlace);

		// Enabling the fullscreen
		SetWindowLongA(hwnd, GWL_STYLE, winStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, kl::window::screen::width, kl::window::screen::height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		// Setting info
		inFull = true;
	}
	else if (inFull && !enable) {
		// Resetting the size
		SetWindowLongA(hwnd, GWL_STYLE, winStyle);
		SetWindowPlacement(hwnd, &winPlace);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		// Setting info
		inFull = false;
	}
}

// Max/min functions
void kl::window::maximize() {
	ShowWindow(this->hwnd, SW_MAXIMIZE);
}
void kl::window::minimize() {
	ShowWindow(this->hwnd, SW_MINIMIZE);
}

// Returns the window size
kl::int2 kl::window::getSize() const {
	RECT clientArea = {};
	GetClientRect(hwnd, &clientArea);
	return kl::int2(clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
}

// Returns the aspect ratio
float kl::window::getAspect() const {
	const kl::int2 winSize = getSize();
	return float(winSize.x) / winSize.y;
}

// Returns the center point of the frame
kl::int2 kl::window::getCenter() const {
	return this->getSize() / 2;
}

// Sets the window title
void kl::window::setTitle(const std::string& data) {
	SetWindowTextA(hwnd, data.c_str());
}

// Sets the window icons
void kl::window::setIcon(const std::string& filePath) {
	// Loading the icon
	HICON loadedIcon = ExtractIconA(nullptr, filePath.c_str(), NULL);
	if (!loadedIcon) {
		std::cout << "WinApi: Could not load an icon file \"" << filePath << "\"";
		std::cin.get();
		exit(69);
	}

	// Sending the icon
	SendMessageA(this->hwnd, WM_SETICON, ICON_BIG, (LPARAM)loadedIcon);
	SendMessageA(this->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)loadedIcon);
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
	StretchDIBits(hdc, position.x, position.y, size.x, size.y, 0, 0, size.x, size.y, toDraw.pointer(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
}
