#pragma once


namespace kl {
	class window {
	public:
		// Input
		kl::keys KEYS;
		kl::mouse MOUSE;

		// User functions
		std::function<void(void)> start;
		std::function<void(void)> update;
		std::function<void(void)> end;

		// Constructor/destructor
		window() {
			KEYS = kl::keys();
			MOUSE = kl::mouse();
			start = []() {};
			update = []() {};
			end = []() {};
			hInstance = GetModuleHandleW(nullptr);
			hwnd = nullptr;
			hdc = nullptr;
			hglrc = nullptr;
			bmpInfo = {};
			wndMsg = {};
		}

		// Window creation
		void startNew(kl::size size, std::string name, bool resizeable = true, bool continuous = false, bool opengl = false) {
			// Converting window name to a wstring
			std::wstring wName = kl::convert::toWString(name);

			// Defining windowapi window class
			registerWindowClass(wName);

			// Creating a window
			createWindow(size, wName, resizeable);

			// Setting up bitmap info
			setupBitmapInfo();

			// Setting up OpenGL context
			if (opengl) {
				setupOpenGL();
			}

			// Starting the update loops
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

		// Returns the window size
		kl::size getSize() {
			RECT clientArea = {};
			GetClientRect(hwnd, &clientArea);
			return kl::size(clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
		}

		// Sets the window title
		void setTitle(std::string data) {
			SetWindowTextA(hwnd, data.c_str());
		}

		// Sets the pixels of the window
		void displayImage(kl::image& toDraw, kl::point position = { 0, 0 }) {
			bmpInfo.bmiHeader.biWidth = toDraw.getWidth();
			bmpInfo.bmiHeader.biHeight = toDraw.getHeight();
			StretchDIBits(hdc, position.x, (toDraw.getHeight() - 1) + position.y, toDraw.getWidth(), -(int)toDraw.getHeight(), 0, 0, toDraw.getWidth(), toDraw.getHeight(), toDraw.getPointer(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		void displayImage(kl::image&& toDraw, kl::point position = { 0, 0 }) {
			displayImage(toDraw, position);
		}

		// Swaps the front and back buffers
		void swapFrameBuffers() {
			SwapBuffers(hdc);
		}

	private:
		// Winapi variables
		HINSTANCE hInstance;
		HWND hwnd;
		HDC hdc;
		HGLRC hglrc;
		BITMAPINFO bmpInfo;
		MSG wndMsg;

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
			if (!RegisterClassExW(&windowClass)) {
				printf("Could not register a window class! Error code: %d\n", GetLastError());
				exit(69);
			}
		}

		// Creates a new window
		void createWindow(kl::size size, std::wstring name, bool resizeable) {
			DWORD windowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
			RECT adjustedWindowSize = { 0, 0, (LONG)size.width, (LONG)size.height };
			AdjustWindowRect(&adjustedWindowSize, windowStyle, FALSE);
			size.width = (adjustedWindowSize.right - adjustedWindowSize.left);
			size.height = (adjustedWindowSize.bottom - adjustedWindowSize.top);
			hwnd = CreateWindowExW(0, name.c_str(), name.c_str(), windowStyle, (kl::constant::ints::ScreenWidth / 2 - size.width / 2), (kl::constant::ints::ScreenHeight / 2 - size.height / 2), size.width, size.height, nullptr, nullptr, hInstance, nullptr);
			if (!hwnd) {
				printf("Could not create a window! Error code: %d\n", GetLastError());
				exit(69);
			}
			ShowWindow(hwnd, SW_SHOW);
			hdc = GetDC(hwnd);
		}

		// Sets up the bitmap properties
		void setupBitmapInfo() {
			bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biBitCount = 24;
			bmpInfo.bmiHeader.biCompression = BI_RGB;
		}

		// Sets up OpenGL context
		void setupOpenGL() {
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
				PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
				32,                   // Colordepth of the framebuffer.
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				24,                   // Number of bits for the depthbuffer
				8,                    // Number of bits for the stencilbuffer
				0,                    // Number of Aux buffers in the framebuffer.
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};
			int pixelFormat = ChoosePixelFormat(hdc, &pfd);
			if (!pixelFormat) {
				printf("Error, could not choose the pixel format %d\n", pixelFormat);
				exit(69);
			}
			SetPixelFormat(hdc, pixelFormat, &pfd);
			hglrc = wglCreateContext(hdc);
			wglMakeCurrent(hdc, hglrc);
			RECT clientArea = {};
			GetClientRect(hwnd, &clientArea);
			glViewport(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom);
		}

		// Handles the windows message
		void handleMessage() {
			switch (wndMsg.message) {
			case WM_KEYDOWN:
				KEYS.setKey(wndMsg.wParam, true);
				break;

			case WM_KEYUP:
				KEYS.setKey(wndMsg.wParam, false);
				break;

			case WM_LBUTTONDOWN:
				MOUSE.lmb = true;
				break;

			case WM_LBUTTONUP:
				MOUSE.lmb = false;
				break;

			case WM_MBUTTONDOWN:
				MOUSE.mmb = true;
				break;

			case WM_MBUTTONUP:
				MOUSE.mmb = false;
				break;

			case WM_RBUTTONDOWN:
				MOUSE.rmb = true;
				break;

			case WM_RBUTTONUP:
				MOUSE.rmb = false;
				break;

			case WM_MOUSEMOVE:
				MOUSE.position = kl::point(GET_X_LPARAM(wndMsg.lParam), GET_Y_LPARAM(wndMsg.lParam));
				break;

			default:
				DispatchMessageW(&wndMsg);
				break;
			}
		}

		// Destroys the contexts
		void cleanup(std::wstring name, bool opengl) {
			if (opengl) {
				wglMakeCurrent(nullptr, nullptr);
				ReleaseDC(hwnd, hdc);
				wglDeleteContext(hglrc);
				hglrc = nullptr;
			}
			UnregisterClassW(name.c_str(), hInstance);
			hdc = nullptr;
			hwnd = nullptr;
		}
	};
}
