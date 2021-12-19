#pragma once


namespace kl {
	struct vao {
		// Constructor/destructor
		vao() {
			glGenVertexArrays(1, &buffer);
		}
		~vao() {
			glDeleteVertexArrays(1, &buffer);
		}

		// Defines the vao
		void define(std::function<void(void)> definition) {
			glBindVertexArray(buffer);
			definition();
			glBindVertexArray(NULL);
		}

		// Draws primitives with vertex data
		void drawArrays(kl::id primitiveType, kl::uint32 vertexCount) {
			glBindVertexArray(buffer);
			glDrawArrays(primitiveType, 0, vertexCount);
			glBindVertexArray(NULL);
		}

		// Draws primitives with vertex and index data
		void drawElements(kl::id primitiveType, kl::uint32 indexCount) {
			glBindVertexArray(buffer);
			glDrawElements(primitiveType, indexCount, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(NULL);
		}

	private:
		// Variables
		kl::id buffer;
	};
}
