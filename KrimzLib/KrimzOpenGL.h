#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "KrimzLib/KrimzTypes.h"


namespace kl {
	class opengl {
	public:
		// Set the whole screen to a given color
		static void ClearScreen(colorf color = { 0, 0, 0, 1 }) {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// Draws a triangle on the screen
		static void DrawTriangle(triangle& tr) {
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < 3; i++) {
				glColor3f(tr.vertices[i].color.r, tr.vertices[i].color.g, tr.vertices[i].color.b);
				glVertex3f(tr.vertices[i].x, tr.vertices[i].y, tr.vertices[i].z);
			}
			glEnd();
		}

		// Flips the front and back hdc buffers
		static void FlipBuffers(HDC hdc) {
			SwapBuffers(hdc);
		}
	};
}
