#pragma once


namespace kl {
	template<typename T> class array {
	public:
		// Constructor and destructor
		array(uint32 count) {
			dataMemory = (T*)calloc(count, sizeof(T));
			if (!dataMemory) {
				printf("Couldn't allocate %d bytes of memory..\n", int(count * sizeof(T)));
				console::WaitFor(' ', true);
				exit(-1);
			}
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

	private:
		T* dataMemory = NULL;
	};
}
