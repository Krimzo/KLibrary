#pragma once
#include <functional>
#include <windows.h>
#include "KrimzLib/incl/OpenGL/glad.h"
#include <gl/GLU.h>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")


/* --- DEBUG --- */
#define ASSERT(x) if (x) __debugbreak();
#define GLCheckError(x) kl::opengl::debug::ClearErrors(); x; ASSERT(kl::opengl::debug::CheckError(__FILE__, __LINE__))


/* --- TYPES --- */
namespace kl
{
	typedef unsigned int id;

	enum class shadertype
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	struct shaderpackage
	{
		std::string vertex = "";
		std::string fragment = "";
	};
}


/* --- CLASS --- */
namespace kl
{
	class opengl
	{
	public:
		class debug
		{
		public:
			// Clears the OpenGL error buffer
			static void ClearErrors()
			{
				while (glGetError());
			}

			// Prints OpenGL from the error buffer
			static bool CheckError(const char* file, int line)
			{
				if (GLenum error = glGetError())
				{
					printf("[OpenGL Error] (0x%x) in file \"%s\" at line %d\n", error, file, line);
					return true;
				}
				return false;
			}
		};

		class old {
		public:
			// Draws a triangle on the screen
			static void DrawTriangle(triangle& tr, vec3& size, vec3& rotation, vec3& position)
			{
				if (tr.textured)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, tr.texture);
					glBegin(GL_TRIANGLES);
					glColor3d(1, 1, 1);
					for (int i = 0; i < 3; i++)
					{
						vertex tempVertex = tr.vertices[i];
						tempVertex.Resize(size);
						tempVertex.Rotate(rotation);
						tempVertex.Translate(position);
						glTexCoord2d(tempVertex.u, tempVertex.v);
						glVertex3d(tempVertex.x, tempVertex.y, tempVertex.z);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				else
				{
					glBegin(GL_TRIANGLES);
					for (int i = 0; i < 3; i++)
					{
						vertex tempVertex = tr.vertices[i];
						tempVertex.Resize(size);
						tempVertex.Rotate(rotation);
						tempVertex.Translate(position);
						glColor3d(tempVertex.color.r, tempVertex.color.g, tempVertex.color.b);
						glVertex3d(tempVertex.x, tempVertex.y, tempVertex.z);
					}
					glEnd();
				}
				
			}
		};

		// Set the whole screen to a given color
		static void ClearBuffers(colord color = { 0, 0, 0, 1 })
		{
			glClearColor((float)color.r, (float)color.g, (float)color.b, (float)color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// Flips the front and back hdc buffers
		static void FlipBuffers(HDC hdc)
		{
			SwapBuffers(hdc);
		}

		// Creates a shader program from 2 given shaders sources
		static kl::id CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
		{
			// Creating and compiling shaders
			kl::id program = glCreateProgram();
			kl::id vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
			kl::id fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

			// Attaching the shaders to the program
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
		static void Delete(kl::id program)
		{
			glDeleteProgram(program);
		}

		// Parses multiple shaders from a single file
		static shaderpackage ShadersFromFile(std::wstring filepath)
		{
			std::istringstream iss(kl::file::ReadText(filepath));

			std::string line;
			std::stringstream ss[2];
			shadertype type = shadertype::NONE;
			while (getline(iss, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = shadertype::VERTEX;

					else if (line.find("fragment") != std::string::npos)
						type = shadertype::FRAGMENT;
				}
				else
				{
					ss[(int)type] << line << '\n';
				}
			}

			return { ss[0].str(), ss[1].str() };
		}

	private:
		// Compiles the given shader source
		static kl::id CompileShader(kl::id type, const std::string& source)
		{
			// Creatings shader
			kl::id shaderID = glCreateShader(type);
			const char* shaderSource = source.c_str();

			// Compiling shader
			glShaderSource(shaderID, 1, &shaderSource, NULL);
			glCompileShader(shaderID);

			// Error checking
			int status;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
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
