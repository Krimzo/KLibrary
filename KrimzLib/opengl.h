#pragma once
#include <windows.h>
#include "KrimzLib/dep/glad/glad.h"
#include "KrimzLib/types.h"


namespace kl {
	class opengl {
	public:
		class debug {
		public:
			// Draws a triangle on the screen for debugging
			static void DrawTriangle(triangle& tr) {
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < 3; i++) {
					glColor3f(tr.vertices[i].color.r, tr.vertices[i].color.g, tr.vertices[i].color.b);
					glVertex3f(tr.vertices[i].x, tr.vertices[i].y, tr.vertices[i].z);
				}
				glEnd();
			}
		};		

		// Set the whole screen to a given color
		static void ClearScreen(colorf color = { 0, 0, 0, 1 }) {
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// Flips the front and back hdc buffers
		static void FlipBuffers(HDC hdc) {
			SwapBuffers(hdc);
		}

		// Creates a shader program from 2 given shaders sources
		static kl::id CreateProgram(const std::string& vertexShader, const std::string& fragmentShader) {
			// Creating and compiling shaders
			kl::id program = glCreateProgram();
			kl::id vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
			kl::id fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

			// Attaching the shaders to the GPU
			glAttachShader(program, vs);
			glAttachShader(program, fs);
			glLinkProgram(program);
			glValidateProgram(program);

			// Deleting compiled shaders
			glDeleteShader(vs);
			glDeleteShader(fs);

			// Returning created shader program id
			return program;
		}

		// Deletes the given shader program
		static void DeleteProgram(kl::id program) {
			glDeleteProgram(program);
		}

	private:
		// Compiles the given shader source
		static kl::id CompileShader(kl::id type, const std::string& source) {
			// Creatings shader
			kl::id shaderID = glCreateShader(type);
			const char* shaderSource = source.c_str();
			
			// Compiling shader
			glShaderSource(shaderID, 1, &shaderSource, NULL);
			glCompileShader(shaderID);

			// Error checking
			int status;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) {
				int messageLen;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &messageLen);
				char* message = new char[messageLen];
				if (!message) exit(-1);
				glGetShaderInfoLog(shaderID, messageLen, &messageLen, message);
				std::cout << "Failed to compile shader!\n" << message << std::endl;
				glDeleteShader(shaderID);
				delete[] message;
				return 0;
			}

			// Returing shader id
			return shaderID;
		}
	};
}
