#pragma once


namespace kl {
	class window {
	public:
		// Input
		kl::key KEY = 0;
		kl::mouse MOUSE = {};

		// Window function calls
		std::function<void(void)> windowStart = []() {};
		std::function<void(void)> windowUpdate = []() {};
		std::function<void(void)> windowEnd = []() {};

		// Window creation
		void startNew(kl::size size, std::string name, bool resizeable = true, bool continuous = false, bool opengl = false) {
			// Converting window name to a wstring
			std::wstring wName = kl::convert::toWString(name);

			// Define windowapi window class
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
			windowClass.lpszClassName = wName.c_str();
			windowClass.hIconSm = nullptr;
			if (!RegisterClassExW(&windowClass)) {
				printf("Couldn't register window class! Last error = %d\n", GetLastError());
				kl::console::waitFor(' ', true);
				exit(69);
			}

			// Create window
			DWORD windowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
			RECT adjustedWindowSize = { 0, 0, (LONG)size.width, (LONG)size.height };
			AdjustWindowRect(&adjustedWindowSize, windowStyle, FALSE);
			size.width = (adjustedWindowSize.right - adjustedWindowSize.left);
			size.height = (adjustedWindowSize.bottom - adjustedWindowSize.top);
			hwnd = CreateWindowExW(0, wName.c_str(), wName.c_str(), windowStyle, (kl::constant::ints::ScreenWidth / 2 - size.width / 2), (kl::constant::ints::ScreenHeight / 2 - size.height / 2), size.width, size.height, nullptr, nullptr, hInstance, nullptr);
			if (!hwnd) {
				printf("Couldn't create window! Last error = %d\n", GetLastError());
				kl::console::waitFor(' ', true);
				exit(69);
			}
			ShowWindow(hwnd, SW_SHOW);
			hdc = GetDC(hwnd);

			// Bitmapinfo setup
			bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biBitCount = 24;
			bmpInfo.bmiHeader.biCompression = BI_RGB;

			// OpenGL setup
			if (opengl) {
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
					exit(69);
				}
				SetPixelFormat(hdc, pixelFormat, &pfd);
				hglrc = wglCreateContext(hdc);
				wglMakeCurrent(hdc, hglrc);
				RECT clientArea = {};
				GetClientRect(hwnd, &clientArea);
				glViewport(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom);
			}

			// Window message loop
			if (continuous) {
				windowStart();
				while (IsWindow(hwnd)) {
					while (PeekMessageW(&wndMsg, hwnd, 0, 0, PM_REMOVE)) {
						handleMessage();
					}
					windowUpdate();
				}
				windowEnd();
			}
			else {
				windowStart();
				while (IsWindow(hwnd)) {
					GetMessageW(&wndMsg, hwnd, 0, 0);
					handleMessage();
					windowUpdate();
				}
				windowEnd();
			}

			// Memory release
			if (opengl) {
				wglMakeCurrent(nullptr, nullptr);
				ReleaseDC(hwnd, hdc);
				wglDeleteContext(hglrc);
				hglrc = nullptr;
			}
			UnregisterClassW(wName.c_str(), hInstance);
			hdc = nullptr;
			hwnd = nullptr;
		}

		// Window destruction
		void stop() {
			PostMessageW(hwnd, WM_CLOSE, 0, 0);
		}
		~window() {
			stop();
		}

		// Getters
		HWND getHWND() {
			return hwnd;
		}
		HDC getHDC() {
			return hdc;
		}
		HGLRC getHGLRC() {
			return hglrc;
		}

		// Returns the window size
		kl::size getSize() {
			RECT clientArea = {};
			GetClientRect(hwnd, &clientArea);
			return kl::size(clientArea.right - clientArea.left, clientArea.bottom - clientArea.top);
		}
		int getWidth() {
			return getSize().width;
		}
		int getHeight() {
			return getSize().height;
		}

		// Sets the window title
		void setTitle(std::string data) {
			SetWindowTextA(hwnd, data.c_str());
		}
		void setTitle(int data) {
			SetWindowTextA(hwnd, std::to_string(data).c_str());
		}
		void setTitle(double data) {
			SetWindowTextA(hwnd, std::to_string(data).c_str());
		}

		// Sets the pixels of the window
		void renderImage(kl::image& toDraw, kl::point position = { 0, 0 }) {
			bmpInfo.bmiHeader.biWidth = toDraw.getWidth();
			bmpInfo.bmiHeader.biHeight = toDraw.getHeight();
			StretchDIBits(hdc, position.x, (toDraw.getHeight() - 1) + position.y, toDraw.getWidth(), -(int)toDraw.getHeight(), 0, 0, toDraw.getWidth(), toDraw.getHeight(), toDraw.getPixelData(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		void renderImage(kl::image&& toDraw, kl::point position = { 0, 0 }) {
			renderImage(toDraw, position);
		}

		// Swaps the front and back buffers
		void swapFrameBuffers() {
			SwapBuffers(hdc);
		}

	private:
		HINSTANCE hInstance = GetModuleHandleW(nullptr);
		HWND hwnd = nullptr;
		HDC hdc = nullptr;
		HGLRC hglrc = nullptr;
		BITMAPINFO bmpInfo = {};
		MSG wndMsg = {};

		// Handles the windows message
		void handleMessage() {
			switch (wndMsg.message) {
			case WM_KEYDOWN:
				KEY = (kl::key)wndMsg.wParam;
				break;

			case WM_KEYUP:
				KEY = 0;
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
				MOUSE.position = kl::point(GET_X_LPARAM(wndMsg.lParam),  GET_Y_LPARAM(wndMsg.lParam));
				break;

			default:
				DispatchMessageW(&wndMsg);
				break;
			}
		}
	};
}
