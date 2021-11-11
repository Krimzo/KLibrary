#pragma once


namespace kl {
	class opengl {
	public:
		// Enables 3D perspective and depth buffer by deafult
		static void Enable3D(double fov, size frameSize, bool enableDepthBuffer = true) {
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(fov, (double)frameSize.width / frameSize.height, 0.01, 100.0);
			if (enableDepthBuffer) {
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}
		}

		// Enables textures
		static void EnableTextures() {
			glEnable(GL_TEXTURE_2D);
		}

		// Creates a new texture and stores it in gpu memory
		static texture NewTexture(image& textureImage) {
			texture createdID = 0;
			glGenTextures(1, &createdID);
			glBindTexture(GL_TEXTURE_2D, createdID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.GetWidth(), textureImage.GetHeight(), 0, 0x80E0/* GL_BGR */, GL_UNSIGNED_BYTE, textureImage.GetRawData());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			return createdID;
		}
		static texture NewTexture(image&& textureImage) {
			return NewTexture(textureImage);
		}

		// Deletes a given texture
		static void DeleteTexture(texture textureID) {
			glDeleteTextures(1, &textureID);
		}

		// Translates and rotates the camera
		static void UpdateCamera(camera camera) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotated(camera.rotation.x, 1, 0, 0);
			glRotated(camera.rotation.y, 0, 1, 0);
			glRotated(camera.rotation.z, 0, 0, 1);
			glTranslated(camera.position.x, camera.position.y, camera.position.z);
		}

		// Clear the frame and depth buffers
		static void ClearBuffers(colord color) {
			glClearColor((float)color.r, (float)color.g, (float)color.b, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// Renders an array of 2D triangles
		static void Render2DTriangles(std::vector<triangle>& triangles, texture textureID) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < triangles.size(); i++) {
				if (triangles[i].textured) {
					glColor3d(1, 1, 1);
					for (int j = 0; j < 3; j++) {
						glTexCoord2d(triangles[i].vertices[j].u, triangles[i].vertices[j].v);
						glVertex3d(triangles[i].vertices[j].x, triangles[i].vertices[j].y, triangles[i].vertices[j].z);
					}
				}
				else {
					for (int j = 0; j < 3; j++) {
						glColor3d(triangles[i].vertices[j].color.r, triangles[i].vertices[j].color.g, triangles[i].vertices[j].color.b);
						glVertex3d(triangles[i].vertices[j].x, triangles[i].vertices[j].y, triangles[i].vertices[j].z);
					}
				}
			}
			glEnd();
		}
		static void Render2DTriangles(std::vector<triangle>&& triangles, texture textureID) {
			Render2DTriangles(triangles, textureID);
		}

		// Renders an array of 3D triangles
		static void Render3DTriangles(std::vector<triangle>& triangles, vec3 position, vec3 rotation, vec3 size, texture textureID) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glPushMatrix();
			glTranslated(position.x, position.y, position.z);
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glScaled(size.x, size.y, size.z);
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < triangles.size(); i++) {
				if (triangles[i].textured) {
					glColor3d(1, 1, 1);
					for (int j = 0; j < 3; j++) {
						glTexCoord2d(triangles[i].vertices[j].u, triangles[i].vertices[j].v);
						glVertex3d(triangles[i].vertices[j].x, triangles[i].vertices[j].y, triangles[i].vertices[j].z);
					}
				}
				else {
					for (int j = 0; j < 3; j++) {
						glColor3d(triangles[i].vertices[j].color.r, triangles[i].vertices[j].color.g, triangles[i].vertices[j].color.b);
						glVertex3d(triangles[i].vertices[j].x, triangles[i].vertices[j].y, triangles[i].vertices[j].z);
					}
				}
			}
			glEnd();
			glPopMatrix();
		}
		static void Render3DTriangles(std::vector<triangle>&& triangles, vec3 position, vec3 rotation, vec3 size, texture textureID) {
			Render3DTriangles(triangles, position, rotation, size, textureID);
		}

		// Flips the front and back hdc buffers
		static void SwapFrameBuffers(HDC hdc) {
			SwapBuffers(hdc);
		}
	};
}
