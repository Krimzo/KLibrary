#pragma once


namespace kl {
	namespace opengl {
		// Enables/disables face culling
		void setFaceCulling(bool enabled, bool cullBack = true, bool clockWise = true) {
			if (enabled) {
				glEnable(GL_CULL_FACE);
				glFrontFace(clockWise ? GL_CW : GL_CCW);
				glCullFace(cullBack ? GL_BACK : GL_FRONT);
			}
			else {
				glDisable(GL_CULL_FACE);
			}
		}

		// Clear the frame and depth buffers
		void clearBuffers(kl::colorf color) {
			static const int clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			glClearColor(color.r, color.g, color.b, 1);
			glClear(clearBit);
		}
	};
}
