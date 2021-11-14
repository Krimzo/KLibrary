#pragma once


namespace kl {
	class window {
	public:
		// Input
		kl::key KEY = 0;
		kl::mouse MOUSE = {};

		// Window function calls
		std::function<void(void)> WindowStart = []() {};
		std::function<void(void)> WindowUpdate = []() {};
		std::function<void(void)> WindowEnd = []() {};

		// Window creation
		void StartNew(kl::size size, std::string name, bool resizeable = true, bool continuous = false, bool opengl = false) {
			// Converting window name to a wstring
			std::wstring wName = kl::convert::ToWString(name);

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
				kl::console::WaitFor(' ', true);
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
				kl::console::WaitFor(' ', true);
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
				WindowStart();
				while (IsWindow(hwnd)) {
					while (PeekMessageW(&wndMsg, hwnd, 0, 0, PM_REMOVE)) {
						HandleMessage();
					}
					WindowUpdate();
				}
				WindowEnd();
			}
			else {
				WindowStart();
				while (IsWindow(hwnd)) {
					GetMessageW(&wndMsg, hwnd, 0, 0);
					HandleMessage();
					WindowUpdate();
				}
				WindowEnd();
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
		void Stop() {
			PostMessageW(hwnd, WM_CLOSE, 0, 0);
		}
		~window() {
			this->Stop();
		}

		// Getters
		HWND GetHWND() {
			return this->hwnd;
		}
		HDC GetHDC() {
			return this->hdc;
		}
		HGLRC GetHGLRC() {
			return this->hglrc;
		}

		// Returns the window size
		kl::size GetSize() {
			RECT clientArea = {};
			GetClientRect(hwnd, &clientArea);
			return { kl::uint32(clientArea.right - clientArea.left), kl::uint32(clientArea.bottom - clientArea.top) };
		}
		int GetWidth() {
			return GetSize().width;
		}
		int GetHeight() {
			return GetSize().height;
		}

		// Sets the window title
		void SetTitle(std::string data) {
			SetWindowTextA(hwnd, data.c_str());
		}
		void SetTitle(int data) {
			SetWindowTextA(hwnd, std::to_string(data).c_str());
		}
		void SetTitle(double data) {
			SetWindowTextA(hwnd, std::to_string(data).c_str());
		}

		// Sets the pixels of the window
		void DisplayImage(kl::image& toDraw, kl::point position = { 0, 0 }) {
			bmpInfo.bmiHeader.biWidth = toDraw.GetWidth();
			bmpInfo.bmiHeader.biHeight = toDraw.GetHeight();
			StretchDIBits(hdc, position.x, (toDraw.GetHeight() - 1) + position.y, toDraw.GetWidth(), -(int)toDraw.GetHeight(), 0, 0, toDraw.GetWidth(), toDraw.GetHeight(), toDraw.GetRawData(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		void DisplayImage(kl::image&& toDraw, kl::point position = { 0, 0 }) {
			DisplayImage(toDraw, position);
		}

	private:
		HINSTANCE hInstance = GetModuleHandleW(nullptr);
		HWND hwnd = nullptr;
		HDC hdc = nullptr;
		HGLRC hglrc = nullptr;
		BITMAPINFO bmpInfo = {};
		MSG wndMsg = {};

		// Handles the windows message
		void HandleMessage() {
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
				MOUSE.position = { GET_X_LPARAM(wndMsg.lParam),  GET_Y_LPARAM(wndMsg.lParam) };
				break;

			default:
				DispatchMessageW(&wndMsg);
				break;
			}
		}
	};
}
