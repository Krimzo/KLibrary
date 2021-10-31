#pragma once


namespace kl {
	template<typename T> class array {
	public:
		// Constructors and destructor
		array(uint64 arraySize = 0, bool canGrow = false) {
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
			if(dataMemory) {
				free(dataMemory);
			}
		}

		// Operator overloading
		T& operator [] (uint64 index) {
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
		void operator <= (T toAdd) {
			SetSize(arraySize + 1);
			dataMemory[arraySize - 1] = toAdd;
		}
		void operator ++ (int ignore) {
			SetSize(arraySize + 1);
		}
		void operator -- (int ignore) {
			if(arraySize) {
				SetSize(arraySize - 1);
			}
		}

		// Returns the current array size
		uint64 GetSize() {
			return arraySize;
		}

		// Sets the array size
		void SetSize(uint64 newSize) {
			// Allocate memory
			dataMemory = (T*)realloc(dataMemory, newSize * sizeof(T));
			if(!dataMemory && newSize) {
				printf("Couldn't allocate %llu bytes of memory..\n", newSize * sizeof(T));
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

		// Returns the first element
		T& Front() {
			return dataMemory[0];
		}

		// Returns the last element
		T& Back() {
			return dataMemory[arraySize - 1];
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
		uint64 Fill(T value) {
			for(uint64 i=0; i<arraySize; i++) {
				dataMemory[i] = value;
			}
			return arraySize;
		} 

        // Returns the index of the first found element or -1 if the element was not found
        int64 Find(T toFind) {
            for(uint64 i=0; i<arraySize; i++) {
                if(dataMemory[i] == toFind) {
                    return (int64)i;
                }
            }
            return -1;
        }

        // Replaces all occurrences of an element with a given replace value
        // Returns the number of replaced elements
        uint64 Replace(T toReplace, T with) {
            uint64 replaceCounter = 0;
            for(uint64 i=0; i<arraySize; i++) {
                if(dataMemory[i] == toReplace) {
                    dataMemory[i] = with;
                    replaceCounter++;
                }
            }
            return replaceCounter;
        }

		// Executes a function on each array element
		void ExecuteOnAll(std::function<void(T& element)> toExecute) {
			for(uint64 i=0; i<arraySize; i++) {
				toExecute(dataMemory[i]);
			}
		}

	private:
		T* dataMemory = NULL;
		uint64 arraySize = 0;
		bool canGrow = false;
	};
}
