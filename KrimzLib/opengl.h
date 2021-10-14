#pragma once
#include <functional>
#include <windows.h>
#include <gl/GLU.h>
#include "KrimzLib/incl/OpenGL/glad.h"
#include "KrimzLib/types.h"
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")


/* --- DEBUG --- */
#define ASSERT(x) if (x) __debugbreak();
#define GLCheckError(x) kl::opengl::debug::ClearErrors(); x; ASSERT(kl::opengl::debug::CheckError(__FILE__, __LINE__))


namespace kl
{
	class opengl
	{
	public:
		// Translates and rotates the camera
		static void SetCameraProperties(vec3& position, vec3& rotation)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glTranslated(position.x, position.y, position.z);
		}

		// Translates, rotates and resizes opengl matrix
		/* !!! Needs to be between glPushMatrix(); and glPopMatrix(); !!! */
		static void SetDrawProperties(vec3& position, vec3& rotation, vec3& size)
		{
			glTranslated(position.x, position.y, position.z);
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glScaled(size.x, size.y, size.z);
		}

		// Draws a triangle on the screen
		/* !!! Needs to be between glBegin(GL_TRIANGLES); and glEnd(); !!! */
		static void DrawTriangle(triangle& tr)
		{
			if (tr.textured)
			{
				glColor3d(1, 1, 1);
				for (int i = 0; i < 3; i++)
				{
					glTexCoord2d(tr.vertices[i].u, tr.vertices[i].v);
					glVertex3d(tr.vertices[i].x, tr.vertices[i].y, tr.vertices[i].z);
				}
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					glColor3d(tr.vertices[i].color.r, tr.vertices[i].color.g, tr.vertices[i].color.b);
					glVertex3d(tr.vertices[i].x, tr.vertices[i].y, tr.vertices[i].z);
				}
			}
		}

		// Sets the given texture to be used by the opengl
		static void BindTexture(id textureID)
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		// Set the whole screen to a given color
		static void ClearBuffers(colorf& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		static void ClearBuffers(colorf&& color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// Flips the front and back hdc buffers
		static void FlipBuffers(HDC hdc)
		{
			SwapBuffers(hdc);
		}
	};
}
