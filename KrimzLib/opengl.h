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

		// Creates a shader program from 2 given shaders sources
		static kl::id CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
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
		static void DeleteShaderProgram(kl::id program)
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
