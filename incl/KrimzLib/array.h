#pragma once


namespace kl {
	template<typename T> class array {
	public:
		// Constructor and destructor
		array(uint32 arraySize = 0, bool canGrow = false) {
			SetSize(arraySize);
			this->canGrow = canGrow;
		}
		~array() {
			free(dataMemory);
		}

		// Operator overloading
		T& operator [] (uint32 index) {
			if(index >= arraySize) {
				if(!canGrow) {
					printf("Error. Trying to access memory outside the non growing array");
					getchar();
					exit(69);
				}
				SetSize(index + 1);
			}
			return dataMemory[index];
		}

		// Returns the current array size
		uint32 GetSize() {
			return arraySize;
		}
		T* GetRawData() {
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

		// Enables the auto array resizing
		void EnableGrowth() {
			canGrow = true;
		}

		// Disables the auto array resizing
		void DisableGrowth() {
			canGrow = false;
		}

        // Returns the index of the first found element or -1 if the element was not found
        int Find(T toFind) {
            for(int i=0; i<arraySize; i++) {
                if(dataMemory[i] == toFind) {
                    return i;
                }
            }
            return -1;
        }

        // Replaces all occurrences of an element with a given replace value
        // Returns the number of replaced elements
        int Replace(T toReplace, T with) {
            int replaceCounter = 0;
            for(int i=0; i<arraySize; i++) {
                if(dataMemory[i] == toReplace) {
                    dataMemory[i] = with;
                    replaceCounter++;
                }
            }
            return replaceCounter;
        }

	private:
		T* dataMemory = NULL;
		uint32 arraySize = 0;
		bool canGrow = false;
	};
}
