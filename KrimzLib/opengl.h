#pragma once
#include <functional>
#include <windows.h>
#include <gl/GLU.h>
#include "KrimzLib/incl/OpenGL/glad.h"
#include "KrimzLib/types.h"
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")


namespace kl
{
	class opengl
	{
	public:
		// Enables 3D perspective and depth buffer by deafult
		static void Enabled3D(double fov, int frameWidth, int frameHeight, bool enableDepthBuffer = true)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(fov, (double)frameWidth / frameHeight, 0.01, 100.0);
			if (enableDepthBuffer)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}
		}

		// Enables textures
		static void EnableTextures()
		{
			glEnable(GL_TEXTURE_2D);
		}

		// Translates and rotates the camera
		static void UpdateCamera(vec3& position, vec3& rotation)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glTranslated(position.x, position.y, position.z);
		}

		// Renders a triangle
		static void RenderTriangle(triangle& tr, vec3& position, vec3& rotation, vec3& size)
		{
			glPushMatrix();
			glTranslated(position.x, position.y, position.z);
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glScaled(size.x, size.y, size.z);
			glBegin(GL_TRIANGLES);
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
			glEnd();
			glPopMatrix();
		}

		// Renders an array of triangles
		static void RenderTriangles(std::vector<triangle>& trs, vec3& position, vec3& rotation, vec3& size)
		{
			glPushMatrix();
			glTranslated(position.x, position.y, position.z);
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glScaled(size.x, size.y, size.z);
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < trs.size(); i++)
			{
				if (trs[i].textured)
				{
					glColor3d(1, 1, 1);
					for (int j = 0; j < 3; j++)
					{
						glTexCoord2d(trs[i].vertices[j].u, trs[i].vertices[j].v);
						glVertex3d(trs[i].vertices[j].x, trs[i].vertices[j].y, trs[i].vertices[j].z);
					}
				}
				else
				{
					for (int j = 0; j < 3; j++)
					{
						glColor3d(trs[i].vertices[j].color.r, trs[i].vertices[j].color.g, trs[i].vertices[j].color.b);
						glVertex3d(trs[i].vertices[j].x, trs[i].vertices[j].y, trs[i].vertices[j].z);
					}
				}
			}
			glEnd();
			glPopMatrix();
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
