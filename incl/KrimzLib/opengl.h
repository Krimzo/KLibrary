#pragma once


namespace kl {
	namespace opengl {
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
