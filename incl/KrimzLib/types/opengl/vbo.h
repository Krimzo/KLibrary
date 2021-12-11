#pragma once


namespace kl {
	struct vbo {
		// Constructor/destructor
		vbo() {
			glGenBuffers(1, &buffer);
		}
		~vbo() {
			glDeleteBuffers(1, &buffer);
		}

		// Binds the buffer
		void bind() {
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
		}

		// Unbinds the buffer
		void unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}

		// Sets the data of the buffer
		void setData(void* data, kl::uint64 byteSize) {
			bind();
			glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_DYNAMIC_DRAW);
			unbind();
		}

		// Sets up the parsing of the array data
		void setParsing(int arrayIndex, int floatsPerAttr, int stride, int offset) {
			bind();
			glEnableVertexAttribArray(arrayIndex);
			glVertexAttribPointer(arrayIndex, floatsPerAttr, GL_FLOAT, false, stride, (void*)offset);
			unbind();
		}

		// Draws the array data to the frame buffer
		void drawArrays(kl::id primitiveType, int vertexCount, int offset) {
			bind();
			glDrawArrays(primitiveType, offset, vertexCount);
			unbind();
		}

	private:
		// Variables
		kl::id buffer;
	};
}
