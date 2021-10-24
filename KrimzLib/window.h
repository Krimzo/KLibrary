#pragma once


namespace kl {
	class window {
	public:
		// Input
		key KEY = 0;
		mouse MOUSE = {};

		// Window function calls
		std::function<void(void)> WindowStart = []() {};
		std::function<void(void)> WindowUpdate = []() {};
		std::function<void(void)> WindowEnd = []() {};

		// Window creation
		void StartNew(size size, std::string name, bool resizeable = true, bool continuous = false, bool opengl = false) {
			// Converting window name to a wstring
			std::wstring wName = convert::ToWString(name);

			// Define windowapi window class
			WNDCLASSEX windowClass = {};
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_OWNDC;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = hInstance;
			windowClass.hIcon = NULL;
			windowClass.hCursor = NULL;
			windowClass.hbrBackground = NULL;
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = wName.c_str();
			windowClass.hIconSm = NULL;
			if (!RegisterClassEx(&windowClass)) {
				printf("Couldn't register window class! Last error = %d\n", GetLastError());
				console::WaitFor(' ', true);
				exit(69);
			}

			// Create window
			DWORD windowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
			RECT adjustedWindowSize = { 0, 0, size.width, size.height };
			AdjustWindowRect(&adjustedWindowSize, windowStyle, NULL);
			size.width = (adjustedWindowSize.right - adjustedWindowSize.left);
			size.height = (adjustedWindowSize.bottom - adjustedWindowSize.top);
			hwnd = CreateWindowEx(NULL, wName.c_str(), wName.c_str(), windowStyle, (constant::ScreenWidth / 2 - size.width / 2), (constant::ScreenHeight / 2 - size.height / 2), size.width, size.height, NULL, NULL, hInstance, NULL);
			if (!hwnd) {
				printf("Couldn't create window! Last error = %d\n", GetLastError());
				console::WaitFor(' ', true);
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
				if (!pixelFormat) { exit(69); }
				SetPixelFormat(hdc, pixelFormat, &pfd);
				hglrc = wglCreateContext(hdc);
				wglMakeCurrent(hdc, hglrc);
				gladLoadGL();	// Load modern OpenGL functions
				RECT clientArea = {};
				GetClientRect(hwnd, &clientArea);
				glViewport(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom);
			}

			// Window message loop
			if (continuous) {
				WindowStart();
				while (IsWindow(hwnd)) {
					while (PeekMessage(&wndMsg, hwnd, 0, 0, PM_REMOVE)) {
						HandleMessage();
					}
					WindowUpdate();
				}
				WindowEnd();
			}
			else {
				WindowStart();
				while (IsWindow(hwnd)) {
					GetMessage(&wndMsg, hwnd, 0, 0);
					HandleMessage();
					WindowUpdate();
				}
				WindowEnd();
			}

			// Memory release
			if (opengl) {
				wglMakeCurrent(NULL, NULL);
				ReleaseDC(hwnd, hdc);
				wglDeleteContext(hglrc);
				hglrc = NULL;
			}
			UnregisterClass(wName.c_str(), hInstance);
			hdc = NULL;
			hwnd = NULL;
		}

		// Window destruction
		void Stop() {
			PostMessage(hwnd, WM_CLOSE, 0, 0);
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
		size GetSize() {
			RECT clientArea = {};
			GetClientRect(hwnd, &clientArea);
			return { clientArea.right - clientArea.left, clientArea.bottom - clientArea.top };
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
		void DisplayImage(image& toDraw, point position = { 0, 0 }) {
			bmpInfo.bmiHeader.biWidth = toDraw.GetWidth();
			bmpInfo.bmiHeader.biHeight = toDraw.GetHeight();
			StretchDIBits(hdc, position.x, (toDraw.GetHeight() - 1) + position.y, toDraw.GetWidth(), -toDraw.GetHeight(), 0, 0, toDraw.GetWidth(), toDraw.GetHeight(), toDraw.GetRawData(), &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		void DisplayImage(image&& toDraw, point position = { 0, 0 }) {
			DisplayImage(toDraw, position);
		}

	private:
		HINSTANCE hInstance = GetModuleHandle(NULL);
		HWND hwnd = NULL;
		HDC hdc = NULL;
		HGLRC hglrc = NULL;
		BITMAPINFO bmpInfo = {};
		MSG wndMsg = {};

		// Handles the windows message
		void HandleMessage() {
			switch (wndMsg.message) {
			case WM_KEYDOWN:
				KEY = (key)wndMsg.wParam;
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
				DispatchMessage(&wndMsg);
				break;
			}
		}
	};
}
