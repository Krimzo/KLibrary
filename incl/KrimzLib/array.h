#pragma once


namespace kl {
	template<typename T> class array {
	public:
		// Constructors and destructor
		array(uint32 arraySize = 0, bool canGrow = false) {
			// Update properties
			this->canGrow = canGrow;

			// Resize
			SetSize(arraySize);
		}
		array(kl::array<T>& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.CanGrow();

			// Resize
			SetSize(arrayToCopy.GetSize());

			// Copy the memory
			memcpy(dataMemory, arrayToCopy.GetRawData(), arraySize * sizeof(T));
		}
		~array() {
			free(dataMemory);
		}

		// Operator overloading
		T& operator [] (uint32 index) {
			// Check if the index is out of the array bounds
			if(index >= arraySize) {
				// Check if the growth is enabled
				if(!canGrow) {
					printf("Error. Trying to access memory outside the non growing array");
					getchar();
					exit(69);
				}

				// Resize if the growth is enabled
				SetSize(index + 1);
			}

			// Return element
			return dataMemory[index];
		}
		void operator = (kl::array<T>& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.CanGrow();

			// Resize
			SetSize(arrayToCopy.GetSize());

			// Copy the memory
			memcpy(dataMemory, arrayToCopy.GetRawData(), arraySize * sizeof(T));
		}

		// Returns the current array size
		uint32 GetSize() {
			return arraySize;
		}

		// Sets the array size
		void SetSize(uint32 newSize) {
			// Allocate memory
			dataMemory = (T*)realloc(dataMemory, newSize * sizeof(T));
			if(!dataMemory) {
				printf("Couldn't allocate %d bytes of memory..\n", int(newSize * sizeof(T)));
                getchar();
                exit(-1);
			}

			// Set newly allocated addresses to 0
			if(newSize > arraySize) {
				memset(dataMemory + arraySize, 0, (newSize - arraySize) * sizeof(T));
			}

			// Update size property
			arraySize = newSize;
		}

		// Returns the pointer to the raw data
		T* GetRawData() {
			return dataMemory;
		}

		// Returns true if the array has growth enabled or false if not
		bool CanGrow() {
			return canGrow;
		}

		// Enables the auto array resizing
		void EnableGrowth() {
			canGrow = true;
		}

		// Disables the auto array resizing
		void DisableGrowth() {
			canGrow = false;
		}

		// Fills the whole array with the given value
		// Returns the number of filled elements
		uint32 Fill(T value) {
			for(int i=0; i<arraySize; i++) {
				dataMemory[i] = value;
			}
			return arraySize;
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

		// Executes a function on each array element
		void ExecuteOnAll(std::function<void(T& element)> toExecute) {
			for(int i=0; i<arraySize; i++) {
				toExecute(dataMemory[i]);
			}
		}

	private:
		T* dataMemory = NULL;
		uint32 arraySize = 0;
		bool canGrow = false;
	};
}
