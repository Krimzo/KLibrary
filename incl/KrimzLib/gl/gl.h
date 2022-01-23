#pragma once


namespace kl {
	typedef unsigned int id;
	namespace gl {
		// Inits an OpenGL context
		HGLRC init(HWND hwnd) {
			// Getting the device context
			HDC hdc = GetDC(hwnd);

			// Creating and setting a pixel format
			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cRedBits = 0;
			pfd.cRedShift = 0;
			pfd.cGreenBits = 0;
			pfd.cGreenShift = 0;
			pfd.cBlueBits = 0;
			pfd.cBlueShift = 0;
			pfd.cAlphaBits = 0;
			pfd.cAlphaShift = 0;
			pfd.cAccumBits = 0;
			pfd.cAccumRedBits = 0;
			pfd.cAccumGreenBits = 0;
			pfd.cAccumBlueBits = 0;
			pfd.cAccumAlphaBits = 0;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
			pfd.cAuxBuffers = 0;
			pfd.iLayerType = PFD_MAIN_PLANE;
			pfd.bReserved = 0;
			pfd.dwLayerMask = 0;
			pfd.dwVisibleMask = 0;
			pfd.dwDamageMask = 0;
			const int pixelFormat = ChoosePixelFormat(hdc, &pfd);
			kl::console::error(!pixelFormat, "OpenGL: Could not choose a pixel format!");
			SetPixelFormat(hdc, pixelFormat, &pfd);

			// Creating an OpenGL context
			HGLRC hglrc = wglCreateContext(hdc);
			kl::console::error(!hglrc, "OpenGL: Falied to create a context!");

			// Binding the created context
			wglMakeCurrent(hdc, hglrc);

			// Loading modern opengl functions
			gladLoadGL();

			// Returing the context
			return hglrc;
		}

		// Uninits an OpenGL context
		void uninit(HGLRC* hglrc) {
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(*hglrc);
			*hglrc = nullptr;
		}

		// Resets the OpenGL viewport
		void resetViewport(HWND hwnd) {
			RECT clientArea = {};
			GetClientRect(hwnd, &clientArea);
			glViewport(clientArea.left, clientArea.top, clientArea.right, clientArea.bottom);
		}

		// Enables/disables face culling
		void setCulling(bool enabled, bool cullBack = true, bool clockWise = false) {
			if (enabled) {
				glEnable(GL_CULL_FACE);
				glFrontFace(clockWise ? GL_CW : GL_CCW);
				glCullFace(cullBack ? GL_BACK : GL_FRONT);
			}
			else {
				glDisable(GL_CULL_FACE);
			}
		}

		// Enables/disables depth testing
		void setDepthTest(bool enabled) {
			if (enabled) {
				glEnable(GL_DEPTH_TEST);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}
		}

		// Enables/disables wireframe rendering
		void setWireframe(bool enabled) {
			if (enabled) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		// Clear the frame and depth buffers
		void clearBuffers(kl::vec3 color) {
			static const int clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			glClearColor(color.x, color.y, color.z, 1);
			glClear(clearBit);
		}
	};
}
