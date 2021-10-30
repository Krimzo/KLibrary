#pragma once


namespace kl {
	template<typename T> class array {
	public:
		// Constructor and destructor
		array(uint32 arraySize) {
			SetSize(arraySize);
		}
		~array() {
			if (dataMemory) {
				free(dataMemory);
			}
		}

		// Operator overloading
		T& operator [] (uint32 i) {
			return dataMemory[i];
		}

		// Returns the current array size
		uint32 GetSize() {
			return arraySize;
		}
		T* GetRaw() {
			return dataMemory;
		}

		// Sets the array size
		void SetSize(uint32 newSize) {
			// Allocate memory
			T* tempBuffer = (T*)calloc(newSize, sizeof(T));
			if (!tempBuffer) {
				printf("Couldn't allocate %d bytes of memory..\n", int(newSize * sizeof(T)));
				getchar();
				exit(-1);
			}
			
			// Check if memory is already allocated
			if (dataMemory) {
				memcpy(tempBuffer, dataMemory, std::min(arraySize, newSize) * sizeof(T));
				free(dataMemory);
			}
			dataMemory = tempBuffer;
			arraySize = newSize;
		}

	private:
		T* dataMemory = NULL;
		uint32 arraySize = 0;
	};
}
