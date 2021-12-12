#pragma once


namespace kl {
	struct ibo {
		// Constructor/destructor
		ibo() {
			glGenBuffers(1, &buffer);
		}
		~ibo() {
			glDeleteBuffers(1, &buffer);
		}

		// Binds the buffer
		void bind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		}

		// Unbinds the buffer
		void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		}

		// Sets the data of the buffer
		void setData(kl::index* data, kl::uint64 byteSize, kl::id usage) {
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, data, usage);
			unbind();
		}

		// Draws primitives with array and index data
		void drawElements(kl::vbo* vertexBuffer, kl::id primitiveType, int indexCount, int offset) {
			vertexBuffer->bind();
			bind();
			glDrawElements(primitiveType, indexCount, GL_UNSIGNED_INT, (void*)offset);
			unbind();
			vertexBuffer->unbind();
		}

	private:
		// Variables
		kl::id buffer;
	};
}
