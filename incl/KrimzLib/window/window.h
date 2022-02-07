#pragma once


namespace kl {
	namespace screen {
		static const int width = GetSystemMetrics(SM_CXSCREEN);
		static const int height = GetSystemMetrics(SM_CYSCREEN);
	}
	class window {
	public:
		// Input
		kl::keys keys;
		kl::mouse mouse;

		// User functions
		Function<void(void)> start = []() {};
		Function<void(void)> update = []() {};
		Function<void(void)> end = []() {};

		// Window creation
		void startNew(kl::ivec2 size, String name, bool resizeable = true, bool continuous = false, bool opengl = false) {
			// Converting window name to a wstring
			std::wstring wName = kl::convert::toWString(name);

			// Registering winapi window class
			registerWindowClass(wName);

			// Creating a window
			createWindow(size, wName, resizeable);

			// Setting up bitmap info
			setupBitmapInfo();

			// Setting up OpenGL context
			if (opengl) {
				initOpenGL();
			}

			// Binding the mouse
			this->mouse.bind(hwnd);

			// Starting the update loops
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			if (continuous) {
				start();
				while (IsWindow(hwnd)) {
					while (PeekMessageW(&wndMsg, hwnd, 0, 0, PM_REMOVE)) {
						handleMessage();
					}
					update();
				}
				end();
			}
			else {
				start();
				while (IsWindow(hwnd)) {
					GetMessageW(&wndMsg, hwnd, 0, 0);
					handleMessage();
					update();
				}
				end();
			}

			// Cleanup
			cleanup(wName, opengl);
		}
		~window() {
			stop();
		}
		void stop() {
			PostMessageW(hwnd, WM_CLOSE, 0, 0);
		}

		// Returns a handle to the window
		HWND getHWND() {
			return hwnd;
		}

		// Sets the fullscreen mode
		void setFullscreen(bool enable) {
			if (!inFull && enable) {
				// Saving old window position
				GetWindowPlacement(hwnd, &winPlace);

				// Enabling the fullscreen
				SetWindowLong(hwnd, GWL_STYLE, winStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(hwnd, HWND_TOP, 0, 0, kl::screen::width, kl::screen::height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

				// Setting info
				inFull = true;
			}
			else if (inFull && !enable) {
				// Resetting the size
				SetWindowLong(hwnd, GWL_STYLE, winStyle);
				SetWindowPlacement(hwnd, &winPlace);
				SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

				// Setting info
				inFull = false;
			}
		}

		// Returns the window size
		kl::ivec2 getSize() {
			RECT clientArea;
			GetClientRect(hwnd, &clientArea);
			return kl::ivec2(clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
		}

		// Returns the aspect ratio
		float getAspect() {
			kl::ivec2 winSize = getSize();
			return (float)winSize.x / winSize.y;
		}

		// Returns the center point of the frame
		kl::ivec2 getCenter() {
			return getSize() / 2;
		}

		// Sets the window title
		void setTitle(const String& data) {
			SetWindowTextA(hwnd, data.c_str());
		}

		// Sets the pixels of the window
		void drawImage(kl::image& toDraw, kl::ivec2 position = { 0, 0 }) {
			bmpInfo.bmiHeader.biWidth = toDraw.getWidth();
			bmpInfo.bmiHeader.biHeight = toDraw.getHeight();
			StretchDIBits(hdc, position.x, (toDraw.getHeight() - 1) + position.y, toDraw.getWidth(), -(int)toDraw.getHeight(), 0, 0, toDraw.getWidth(), toDraw.getHeight(), toDraw.pointer(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		void drawImage(kl::image&& toDraw, kl::ivec2 position = { 0, 0 }) {
			drawImage(toDraw, position);
		}

		// Binds the OpenGL contex of the window
		void makeCurrentGL() {
			wglMakeCurrent(hdc, hglrc);
		}

		// Resets the OpenGL viewport
		void resetViewport() {
			kl::gl::resetViewport(hwnd);
		}

		// Swaps the front and back buffers
		void swapBuffers() {
			SwapBuffers(hdc);
		}

	private:
		// Winapi variables
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		HWND hwnd = nullptr;
		HDC hdc = nullptr;
		HGLRC hglrc = nullptr;
		BITMAPINFO bmpInfo = {};
		MSG wndMsg = {};

		// Fullscreen data
		bool inFull = false;
		DWORD winStyle = NULL;
		WINDOWPLACEMENT winPlace = {};

		// Registers a new window class
		void registerWindowClass(std::wstring name) {
			WNDCLASSEXW windowClass = {};
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_OWNDC;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = hInstance;
			windowClass.hIcon = nullptr;
			windowClass.hCursor = nullptr;
			windowClass.hbrBackground = nullptr;
			windowClass.lpszMenuName = nullptr;
			windowClass.lpszClassName = name.c_str();
			windowClass.hIconSm = nullptr;
			kl::console::error(!RegisterClassExW(&windowClass), "WinApi: Could not register a window class!");
		}

		// Creates a new window
		void createWindow(kl::ivec2 size, std::wstring name, bool resizeable) {
			// Setting the window properties
			winStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
			RECT adjustedWindowSize = { 0, 0, (LONG)size.x, (LONG)size.y };
			AdjustWindowRect(&adjustedWindowSize, winStyle, FALSE);
			size.x = (adjustedWindowSize.right - adjustedWindowSize.left);
			size.y = (adjustedWindowSize.bottom - adjustedWindowSize.top);

			// Creating the window
			hwnd = CreateWindowExW(0, name.c_str(), name.c_str(), winStyle, (kl::screen::width / 2 - size.x / 2), (kl::screen::height / 2 - size.y / 2), size.x, size.y, nullptr, nullptr, hInstance, nullptr);
			kl::console::error(!hwnd, "WinApi: Could not create a window!");

			// Setting and getting window info
			ShowWindow(hwnd, SW_SHOW);
			hdc = GetDC(hwnd);

			// Saving a complete window style
			winStyle = GetWindowLong(hwnd, GWL_STYLE);
		}

		// Sets up the bitmap properties
		void setupBitmapInfo() {
			bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biBitCount = 24;
			bmpInfo.bmiHeader.biCompression = BI_RGB;
		}

		// Sets up OpenGL context
		void initOpenGL() {
			// Init OpenGL
			hglrc = kl::gl::init(hwnd);

			// Setting the viewport size
			kl::gl::resetViewport(hwnd);
		}

		// Handles the windows message
		void handleMessage() {
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
				this->mouse.position = kl::ivec2(GET_X_LPARAM(wndMsg.lParam), GET_Y_LPARAM(wndMsg.lParam));
				break;

			default:
				DispatchMessageW(&wndMsg);
				break;
			}
		}

		// Destroys the contexts
		void cleanup(std::wstring name, bool opengl) {
			// Destroying the opengl context
			if (opengl) {
				kl::gl::uninit(&hglrc);
			}

			// Unregistering the window class
			UnregisterClassW(name.c_str(), hInstance);
			hdc = nullptr;
			hwnd = nullptr;
		}
	};
}
