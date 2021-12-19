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

		// Sets the data of the buffer
		void setData(void* data, kl::uint64 byteSize, kl::id usage) {
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, byteSize, data, usage);
		}

		// Sets up the parsing of the array data
		void setParsing(int arrayIndex, kl::id dataType, int sizePerAttr, int stride, int offset) {
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glEnableVertexAttribArray(arrayIndex);
			glVertexAttribPointer(arrayIndex, sizePerAttr, dataType, false, stride, (void*)offset);
		}

	private:
		// Variables
		kl::id buffer;
	};
}
