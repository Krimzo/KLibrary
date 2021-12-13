#pragma once


namespace kl {
	namespace opengl {
		// Setups perspective for 3D rendering
		void setup3D(float fov, kl::size frameSize) {
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(fov, double(frameSize.width) / frameSize.height, 0.01, 100.0);
		}
		
		// Enables/disables depth buffering
		void setDepthTest(bool enable) {
			if (enable) {
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}
		}

		// Enables/disables face culling
		void setFaceCulling(bool enabled, bool clockWiseFront = true, bool cullBack = true) {
			if (enabled) {
				glEnable(GL_CULL_FACE);
				glFrontFace(clockWiseFront ? GL_CW : GL_CCW);
				glCullFace(cullBack ? GL_BACK : GL_FRONT);
			}
			else {
				glDisable(GL_CULL_FACE);
			}
		}

		// Enables/disables textures
		void setTextures(bool enable) {
			if (enable) {
				glEnable(GL_TEXTURE_2D);
			}
			else {
				glDisable(GL_TEXTURE_2D);
			}
		}

		// Creates a new texture and stores it in gpu memory
		kl::texture newTexture(kl::image& textureImage) {
			kl::texture createdID = 0;
			glGenTextures(1, &createdID);
			glBindTexture(GL_TEXTURE_2D, createdID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.getWidth(), textureImage.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureImage.getPointer());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			return createdID;
		}
		kl::texture newTexture(kl::image&& textureImage) {
			return newTexture(textureImage);
		}

		// Deletes a given texture
		void deleteTexture(kl::texture textureID) {
			glDeleteTextures(1, &textureID);
		}

		// Clear the frame and depth buffers
		void clearBuffers(kl::colorf color) {
			static const int clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			glClearColor(color.r, color.g, color.b, 1);
			glClear(clearBit);
		}

		// Resets the modelview matrix
		void resetModelMatrix() {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		// Translates and rotates the camera
		void updateCamera(kl::camera camera) {
			glMatrixMode(GL_MODELVIEW);
			//glRotatef(-camera.rotation.x, 1, 0, 0);
			//glRotatef(-camera.rotation.y, 0, 1, 0);
			//glRotatef(-camera.rotation.z, 0, 0, 1);
			glTranslatef(-camera.position.x, -camera.position.y, -camera.position.z);
		}

		// Renders an array of 3D triangles
		void renderTriangles(std::vector<kl::triangle>& triangles, kl::vec3 position, kl::vec3 rotation, kl::vec3 size, kl::texture textureID) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glRotatef(rotation.x, 1, 0, 0);
			glRotatef(rotation.y, 0, 1, 0);
			glRotatef(rotation.z, 0, 0, 1);
			glScalef(size.x, size.y, size.z);
			glBegin(GL_TRIANGLES);
			glColor3f(1, 1, 1);
			for (int i = 0; i < triangles.size(); i++) {
				for (int j = 0; j < 3; j++) {
					glTexCoord2f(triangles[i].vertices[j].texture.x, triangles[i].vertices[j].world.y);
					glVertex3f(triangles[i].vertices[j].world.x, triangles[i].vertices[j].world.y, triangles[i].vertices[j].world.z);
				}
			}
			glEnd();
			glPopMatrix();
		}
		void renderTriangles(std::vector<kl::triangle>&& triangles, kl::vec3 position, kl::vec3 rotation, kl::vec3 size, kl::texture textureID) {
			renderTriangles(triangles, position, rotation, size, textureID);
		}
	};
}
