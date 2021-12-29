#pragma once


namespace kl {
	namespace gl {
		struct uniform {
			uniform() {}
			uniform(kl::id programID, kl::id uniformID) {
				this->programID = programID;
				this->uniformID = uniformID;
			}

			// Setts the data of an uniform
			void setData(int data) {
				glUseProgram(programID);
				glUniform1i(uniformID, data);
			}
			void setData(float data) {
				glUseProgram(programID);
				glUniform1f(uniformID, data);
			}
			void setData(kl::vec2 data) {
				glUseProgram(programID);
				glUniform2f(uniformID, data.x, data.y);
			}
			void setData(kl::vec3 data) {
				glUseProgram(programID);
				glUniform3f(uniformID, data.x, data.y, data.z);
			}
			void setData(kl::vec4 data) {
				glUseProgram(programID);
				glUniform4f(uniformID, data.x, data.y, data.z, data.w);
			}
			void setData(kl::mat4& data) {
				glUseProgram(programID);
				glUniformMatrix4fv(uniformID, 1, true, data.pointer());
			}
			void setData(kl::mat4&& data) {
				setData(data);
			}

		private:
			kl::id programID = NULL;
			kl::id uniformID = NULL;
		};
	}
}
