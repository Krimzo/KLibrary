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
		void genBuff() {
			if (!depthFB) {
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
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);

				// Chechink the frame buffer status
				kl::id checkStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (checkStatus != GL_FRAMEBUFFER_COMPLETE) {
					printf("Shadow frame buffer error, status: 0x%x\n", checkStatus);
				}

				// Binding the default frame buffer
				glBindFramebuffer(GL_FRAMEBUFFER, NULL);

				// Compiling the shaders and getting uniforms
				depth_sha = new kl::shaders(vertSource, fragSource);
				light_vp_uni = depth_sha->getUniform("light_vp");
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

		// Returns the light vp matrix
		kl::mat4 matrix() {
			const float horizRange = 10;
			const float vertRange = 10;
			const float nearRange = 0.1;
			const float farRange = 10;

			kl::vec3 eye = direction.negate();
			kl::vec3 center = kl::vec3(0, 0, 0);
			kl::vec3 up = kl::vec3(0, 1, 0);

			// Return the vp matrix
			return kl::mat4::ortho(-horizRange, horizRange, -vertRange, vertRange, nearRange, farRange) * kl::mat4::lookAt(eye, center, up);
		}

		// Setups the depth rendering
		void renderStart() {
			// Setting the viewport size
			glViewport(0, 0, mapSize, mapSize);

			// Binding the shadow frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, depthFB);

			// Clearing the depth data
			glClear(GL_DEPTH_BUFFER_BIT);

			// Setting the light vp uni
			light_vp_uni.setData(this->matrix());
		}

		// Finishes the depth rendering
		void renderEnd(kl::size frameSize) {
			// Binding the defualt frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, NULL);

			// Resetting the viewport
			glViewport(0, 0, frameSize.width, frameSize.height);

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

		// Shadow map size
		static const int mapSize = 2048;

		// Shadow shaders
		kl::shaders* depth_sha = nullptr;
		kl::uniform light_vp_uni;
		kl::uniform w_uni;

		// Shadow shader sources
		static std::string vertSource;
		static std::string fragSource;
	};

	// Shadow vertex shader
	std::string kl::direct::vertSource = R"(
		#version 330

		layout (location = 0) in vec3 world;

		uniform mat4 light_vp;
		uniform mat4 w;

		void main() {
			gl_Position = light_vp * w * vec4(world, 1);
		}
	)";

	// Shadow fragment shader
	std::string kl::direct::fragSource = R"(
		#version 330

		void main() {} 
	)";
}
