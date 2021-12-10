#pragma once


#define GL_BGR 0x80E0

namespace kl {
	namespace opengl {
		// Setups perspective for 3D rendering
		void setup3D(double fov, kl::size frameSize) {
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
		void setFaceCulling(bool enable, bool backFace = true) {
			if (enable) {
				glEnable(GL_CULL_FACE);
				glCullFace(backFace ? GL_BACK : GL_FRONT);
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.getWidth(), textureImage.getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureImage.getPixelData());
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
		void clearBuffers(kl::color color) {
			glClearColor(color.r * kl::constant::floats::toFloatColor, color.g * kl::constant::floats::toFloatColor, color.b * kl::constant::floats::toFloatColor, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// Resets the modelview matrix
		void resetModelMatrix() {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		// Translates and rotates the camera
		void updateCamera(kl::camera camera) {
			glMatrixMode(GL_MODELVIEW);
			glRotated(-camera.rotation.x, 1, 0, 0);
			glRotated(-camera.rotation.y, 0, 1, 0);
			glRotated(-camera.rotation.z, 0, 0, 1);
			glTranslated(-camera.position.x, -camera.position.y, -camera.position.z);
		}

		// Renders an array of 3D triangles
		void renderTriangles(std::vector<kl::triangle>& triangles, kl::vec3 position, kl::vec3 rotation, kl::vec3 size, kl::texture textureID) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslated(position.x, position.y, position.z);
			glRotated(rotation.x, 1, 0, 0);
			glRotated(rotation.y, 0, 1, 0);
			glRotated(rotation.z, 0, 0, 1);
			glScaled(size.x, size.y, size.z);
			glBegin(GL_TRIANGLES);
			glColor3d(1, 1, 1);
			for (int i = 0; i < triangles.size(); i++) {
				for (int j = 0; j < 3; j++) {
					glTexCoord2d(triangles[i].vertices[j].u, triangles[i].vertices[j].v);
					glVertex3d(triangles[i].vertices[j].x, triangles[i].vertices[j].y, triangles[i].vertices[j].z);
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
