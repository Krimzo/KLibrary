#pragma once

#include <windows.h>

#include "KrimzLib/vecmath/vec3.h"
#include "KrimzLib/gl/glad.h"

#pragma comment(lib, "opengl32.lib")


namespace kl {
	typedef unsigned int id;
	namespace gl {
		// Inits an OpenGL context
		HGLRC init(HWND hwnd);

		// Uninits an OpenGL context
		void uninit(HGLRC* hglrc);

		// Resets the OpenGL viewport
		void resetViewport(HWND hwnd);

		// Enables/disables face culling
		void setCulling(bool enabled, bool cullBack = true, bool clockWise = false);

		// Enables/disables depth testing
		void setDepthTest(bool enabled);

		// Enables/disables wireframe rendering
		void setWireframe(bool enabled);

		// Clear the frame and depth buffers
		void clearBuffers(const kl::vec3& color);
	};
}
