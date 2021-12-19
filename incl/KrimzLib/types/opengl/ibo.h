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

		// Sets the data of the buffer
		void setData(kl::index* data, kl::uint64 byteSize, kl::id usage) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, data, usage);
		}

	private:
		// Variables
		kl::id buffer;
	};
}
