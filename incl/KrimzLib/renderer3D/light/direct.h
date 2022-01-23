#pragma once


namespace kl {
	struct direct {
		kl::vec3 color;
		kl::vec3 direction;
		float intensity = 0;

		// Returns the true light color
		kl::vec3 getCol() {
			return color * intensity;
		}

		// Returns the direction of light
		kl::vec3 getDir() {
			return direction.normalize();
		}

		// Generates the shadow buffers
		void genBuff(int mapSize) {
			if (!depthFB) {
				// Upadating the map size
				this->mapSize = mapSize;

				// Generating the shadow frame buffer
				glGenFramebuffers(1, &depthFB);

				// Generating the shadow depth texture
				glGenTextures(1, &depthMap);
				glBindTexture(GL_TEXTURE_2D, depthMap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mapSize, mapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

				// Fixing the borders
				float borderColor[] = { 1, 1, 1, 1 };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

				// Attaching the shadow map to the shadow frame buffer
				glBindFramebuffer(GL_FRAMEBUFFER, depthFB);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

				// Disabling the color buffers
				glDrawBuffer(NULL);
				glReadBuffer(NULL);

				// Chechink the frame buffer status
				kl::id checkStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (checkStatus != GL_FRAMEBUFFER_COMPLETE) {
					printf("Shadow frame buffer error, status: 0x%x\n", checkStatus);
				}

				// Binding the default frame buffer
				glBindFramebuffer(GL_FRAMEBUFFER, NULL);

				// Compiling the shaders and getting uniforms
				depth_sha = new kl::shaders(vertSource, fragSource);
				sunVP_uni = depth_sha->getUniform("sunVP");
				w_uni = depth_sha->getUniform("w");
			}
		}

		// Destroys the shadow buffers
		void delBuff() {
			if (depthFB) {
				// Deleting the the shadow frame buffer
				glDeleteFramebuffers(1, &depthFB);
				depthFB = NULL;

				// Deleting the shadow map
				glGenTextures(1, &depthMap);
				depthMap = NULL;

				// Deleting the shaders
				delete depth_sha;
				depth_sha = nullptr;
			}
		}

		// Calculates the light vp matrix
		void calcMat(kl::camera& cam) {
			// Calculating the near points
			const float nearDist = cam.nearPlane;
			const float Hnear = 2 * tan(kl::convert::toRadians(cam.fov) * 0.5f) * nearDist;
			const float Wnear = Hnear * cam.aspect;
			kl::vec3 un = cam.getUp() * Hnear * 0.5;
			kl::vec3 rn = cam.getRight() * Wnear * 0.5;
			kl::vec3 centerNear = cam.position + cam.getForward() * nearDist;
			kl::vec3 topLeftNear = centerNear + un - rn;
			kl::vec3 topRightNear = centerNear + un + rn;
			kl::vec3 bottomLeftNear = centerNear - un - rn;
			kl::vec3 bottomRightNear = centerNear - un + rn;

			// Calculating the far points
			const float farDist = cam.shadowD;
			const float Hfar = 2 * tan(kl::convert::toRadians(cam.fov) * 0.5f) * farDist;
			const float Wfar = Hfar * cam.aspect;
			kl::vec3 uf = cam.getUp() * Hfar * 0.5f;
			kl::vec3 rf = cam.getRight() * Wfar * 0.5f;
			kl::vec3 centerFar = cam.position + cam.getForward() * farDist;
			kl::vec3 topLeftFar = centerFar + uf - rf;
			kl::vec3 topRightFar = centerFar + uf + rf;
			kl::vec3 bottomLeftFar = centerFar - uf - rf;
			kl::vec3 bottomRightFar = centerFar - uf + rf;

			// Calculating the view center
			kl::vec3 frustumCenter = (centerFar - centerNear) * 0.5f;

			// Calculating the light view matrix
			kl::mat4 view = kl::mat4::lookAt(getDir().negate(), vec3(0, 0, 0), vec3(0, 0, 1));

			// Transforming the frustum points to the light view space
			std::vector<vec4> lightViewFrust{
				view * kl::vec4(bottomRightNear, 1),
				view * kl::vec4(topRightNear, 1),
				view * kl::vec4(bottomLeftNear, 1),
				view * kl::vec4(topLeftNear, 1),
				view * kl::vec4(bottomRightFar, 1),
				view * kl::vec4(topRightFar, 1),
				view * kl::vec4(bottomLeftFar, 1),
				view * kl::vec4(topLeftFar, 1)
			};

			// Finding the min and max points that generate the bounding box
			kl::vec3 min(INFINITY, INFINITY, INFINITY);
			kl::vec3 max(-INFINITY, -INFINITY, -INFINITY);
			for (int i = 0; i < lightViewFrust.size(); i++) {
				if (lightViewFrust[i].x < min.x) {
					min.x = lightViewFrust[i].x;
				}
				if (lightViewFrust[i].y < min.y) {
					min.y = lightViewFrust[i].y;
				}
				if (lightViewFrust[i].z < min.z) {
					min.z = lightViewFrust[i].z;
				}

				if (lightViewFrust[i].x > max.x) {
					max.x = lightViewFrust[i].x;
				}
				if (lightViewFrust[i].y > max.y) {
					max.y = lightViewFrust[i].y;
				}
				if (lightViewFrust[i].z > max.z) {
					max.z = lightViewFrust[i].z;
				}
			}

			// Calculating the ortho projection matrix
			kl::mat4 proj = kl::mat4::ortho(min.x, max.x, min.y, max.y, -max.z - 3 * cam.shadowD, -min.z);

			// Setting the sun view/projection matrix
			sunVP = proj * view;
		}

		// Returns the light vp matrix
		kl::mat4 matrix() {
			return sunVP;
		}

		void render(kl::window* win, std::function<void()> toRender) {
			// Setting the viewport size
			glViewport(0, 0, mapSize, mapSize);

			// Binding the shadow frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, depthFB);

			// Clearing the depth data
			glClear(GL_DEPTH_BUFFER_BIT);

			// Setting the light vp uni
			sunVP_uni.setData(this->matrix());

			// Rendering
			toRender();

			// Binding the defualt frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, NULL);

			// Resetting the viewport
			win->resetViewport();

			// Binding the shadow map
			this->bindMap(GL_TEXTURE1);
		}

		// Sets the object world transform matrix
		void setWMat(kl::mat4& wMat) {
			w_uni.setData(wMat);
		}
		void setWMat(kl::mat4&& wMat) {
			setWMat(wMat);
		}

		// Binds the shadow map
		void bindMap(kl::id textureID) {
			glActiveTexture(textureID);
			glBindTexture(GL_TEXTURE_2D, depthMap);
		}

	private:
		// Shadow buffers
		kl::id depthFB = NULL;
		kl::id depthMap = NULL;

		// Light vp matrix
		kl::mat4 sunVP;

		// Shadow map size
		int mapSize = 0;

		// Shadow shaders
		kl::shaders* depth_sha = nullptr;
		kl::uniform sunVP_uni;
		kl::uniform w_uni;

		// Shadow shader sources
		static std::string vertSource;
		static std::string fragSource;
	};

	// Shadow vertex shader
	std::string kl::direct::vertSource = R"(
		#version 330

		layout (location = 0) in vec3 world;

		uniform mat4 sunVP;
		uniform mat4 w;

		void main() {
			gl_Position = sunVP * w * vec4(world, 1);
		}
	)";

	// Shadow fragment shader
	std::string kl::direct::fragSource = R"(
		#version 330

		void main() {} 
	)";
}
