#pragma once
#include <iostream>
#include <functional>
#include <thread>
#include <string>
#include <windows.h>
#include <windowsx.h>
#include "KrimzLib/dep/glad/glad.h"
#include "KrimzLib/types.h"
#include "KrimzLib/constants.h"


namespace kl {
	class window {
	public:
		// Input properties
		WPARAM KEY = 0;
		bool LMB = false;
		bool RMB = false;
		point MOUSE = {};

		// OpenGL calls
		std::function<void(void)> OpenGLStart = []() {};
		std::function<void(void)> OpenGLUpdate = []() {};

		// Window creation and deletion
		// You need to link "opengl32.lib" if you want to use OpenGL
		void New(int windowWidth, int windowHeight, const wchar_t* windowName, bool resizeable = true, bool useOpenGL = false) {
			if (!windowCreated) {
				// Start a new window thread
				std::thread windowThread([&]() {
					// Define windowapi window class
					WNDCLASS windowClass = {};
					windowClass.lpfnWndProc = DefWindowProc;
					windowClass.hInstance = hInstance;
					windowClass.lpszClassName = windowName;
					windowClass.style = CS_OWNDC;
					RegisterClass(&windowClass);

					// Create window
					DWORD windowStyle = resizeable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
					RECT adjustedWindowSize = { 0, 0, windowWidth, windowHeight };
					AdjustWindowRectEx(&adjustedWindowSize, windowStyle, NULL, NULL);
					windowWidth = (adjustedWindowSize.right - adjustedWindowSize.left);
					windowHeight = (adjustedWindowSize.bottom - adjustedWindowSize.top);
					hwnd = CreateWindowEx(NULL, windowName, windowName, windowStyle, (constant::ScreenWidth / 2 - windowWidth / 2), (constant::ScreenHeight / 2 - windowHeight / 2), windowWidth, windowHeight, NULL, NULL, hInstance, NULL);
					if (!hwnd) { exit(69); }
					ShowWindow(hwnd, SW_SHOW);
					hdc = GetDC(hwnd);
					windowCreated = true;

					// Bitmapinfo setup
					bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
					bitmapInfo.bmiHeader.biPlanes = 1;
					bitmapInfo.bmiHeader.biBitCount = 32;
					bitmapInfo.bmiHeader.biCompression = BI_RGB;

					// OpenGL setup
					if (useOpenGL) {
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
						gladLoadGL();		// Loads modern OpenGL functions
						RECT clientArea = {};
						GetClientRect(hwnd, &clientArea);
						glViewport(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom);
					}

					// Window message loop
					if (useOpenGL) {
						OpenGLStart();
						while (IsWindow(hwnd)) {
							while (PeekMessage(&windowMessage, hwnd, 0, 0, PM_REMOVE)) {
								HandleMessage();
							}
							OpenGLUpdate();
						}
					}
					else {
						while (IsWindow(hwnd)) {
							GetMessage(&windowMessage, hwnd, 0, 0);
							HandleMessage();
						}
					}

					// Memory release
					if (useOpenGL) {
						wglMakeCurrent(NULL, NULL);
						ReleaseDC(hwnd, hdc);
						wglDeleteContext(hglrc);
						hglrc = NULL;
					}
					UnregisterClass(windowName, hInstance);
					hdc = NULL;
					hwnd = NULL;
					windowCreated = false;
					});
				windowThread.detach();
				while (!windowCreated);
			}
		}
		void Delete() {
			if (windowCreated) {
				// Wait until window/thread gets destroyed
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				while (windowCreated);
			}
		}
		~window() {
			this->Delete();
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
		void DisplayBitmap(bitmap& toDraw, point position = { 0, 0 }) {
			bitmapInfo.bmiHeader.biWidth = toDraw.GetWidth();
			bitmapInfo.bmiHeader.biHeight = toDraw.GetHeight();
			StretchDIBits(hdc, position.x, (toDraw.GetHeight() - 1) + position.y, toDraw.GetWidth(), -toDraw.GetHeight(), 0, 0, toDraw.GetWidth(), toDraw.GetHeight(), toDraw.GetPixelData(), &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}

	private:
		// Winapi properties
		HINSTANCE hInstance = GetModuleHandle(NULL);
		HWND hwnd = NULL;
		HDC hdc = NULL;
		BITMAPINFO bitmapInfo = {};
		MSG windowMessage = {};
		bool windowCreated = false;

		// OpenGL properties
		HGLRC hglrc = NULL;

		// Handles the windows message
		void HandleMessage() {
			switch (windowMessage.message) {
			case WM_KEYDOWN:
				KEY = windowMessage.wParam;
				break;

			case WM_KEYUP:
				KEY = 0;
				break;

			case WM_LBUTTONDOWN:
				LMB = true;
				break;

			case WM_LBUTTONUP:
				LMB = false;
				break;

			case WM_RBUTTONDOWN:
				RMB = true;
				break;

			case WM_RBUTTONUP:
				RMB = false;
				break;

			case WM_MOUSEMOVE:
				MOUSE = { GET_X_LPARAM(windowMessage.lParam),  GET_Y_LPARAM(windowMessage.lParam) };
				break;

			default:
				DispatchMessage(&windowMessage);
				break;
			}
		}
	};
}
