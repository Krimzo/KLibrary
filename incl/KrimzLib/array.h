#pragma once


namespace kl {
	template<typename T> class array {
	public:
		// Constructors and destructor
		array(kl::uint64 arraySize = 0, bool canGrow = false) {
			// Update properties
			this->canGrow = canGrow;

			// Resize
			Resize(arraySize);
		}
		array(kl::array<T>& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.Growth();

			// Resize
			Resize(arrayToCopy.Size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.Size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		array(kl::array<T>&& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.Growth();

			// Resize
			Resize(arrayToCopy.Size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.Size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		array(std::initializer_list<T> listOfElements) {
			// Resize
			Resize(listOfElements.size());

			// Copy data from the initialiser list to the array memory
			for (kl::uint64 i = 0; i < listOfElements.size(); i++) {
				arrayData[i] = listOfElements.begin()[i];
			}
		}
		~array() {
			delete[] arrayData;
		}

		// Operator overloading
		T& operator [] (kl::uint64 index) {
			// Check if the index is out of the array bounds
			if (index > (arraySize - 1) || !arraySize) {
				// Check if the growth is enabled
				if (!canGrow) {
					printf("Error. Trying to access memory outside the non growing array\n");
					kl::console::WaitFor(' ', true);
					exit(69);
				}

				// Resize if the growth is enabled
				Resize(index + 1);
			}

			// Return element
			return arrayData[index];
		}
		void operator = (kl::array<T>& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.Growth();

			// Resize
			Resize(arrayToCopy.Size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.Size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		void operator = (kl::array<T>&& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.Growth();

			// Resize
			Resize(arrayToCopy.Size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.Size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		void operator = (std::initializer_list<T> listOfElements) {
			// Resize
			Resize(listOfElements.size());

			// Copy data from the initialiser list to the array memory
			for (kl::uint64 i = 0; i < listOfElements.size(); i++) {
				arrayData[i] = listOfElements.begin()[i];
			}
		}
		void operator ++ (int ignore) {
			Resize(arraySize + 1);
		}
		void operator -- (int ignore) {
			if (arraySize) {
				Resize(arraySize - 1);
			}
		}
		void operator <= (T toAdd) {
			Resize(arraySize + 1);
			Back() = toAdd;
		}

		// Returns the current array size
		kl::uint64 Size() {
			return arraySize;
		}

		// Changes the array size
		void Resize(kl::uint64 newSize) {
			// Allocate temp memory
			T* tempMemory = new T[newSize];
			if (!tempMemory) {
				printf("Couldn't allocate %llu bytes of memory..\n", newSize * sizeof(T));
				console::WaitFor(' ', true);
                exit(-1);
			}

			// Set newly allocated addresses to the default values
			if (newSize > arraySize) {
				for (kl::uint64 i = arraySize; i < newSize; i++) {
					tempMemory[i] = {};
				}
			}

			// Copy the data from old to temp memory
			kl::uint64 minSize = std::min(arraySize, newSize);
			for (kl::uint64 i = 0; i < minSize; i++) {
				tempMemory[i] = arrayData[i];
			}

			// Free old memory and update array properties
			delete[] arrayData;
			arrayData = tempMemory;
			arraySize = newSize;
		}

		// Sets the size to 0
		void Clear() {
			Resize(0);
		}

		// Returns the first element
		T& Front() {
			if (arraySize) {
				return arrayData[0];
			}
			printf("Error. Trying to access memory outside the non growing array\n");
			kl::console::WaitFor(' ', true);
			exit(69);
		}

		// Returns the last element
		T& Back() {
			if (arraySize) {
				return arrayData[arraySize - 1];
			}
			printf("Error. Trying to access memory outside the non growing array\n");
			kl::console::WaitFor(' ', true);
			exit(69);
		}

		// Returns the pointer to the raw data
		T* RawData() {
			return arrayData;
		}

		// Returns true if the array has growth enabled or false if not
		bool Growth() {
			return canGrow;
		}
		void Growth(bool canGrow) {
			this->canGrow = canGrow;
		}

		// Inserts an array of elements on the given index
		void Insert(kl::uint64 index, kl::array<T>& toInsert) {
			// Update to fit the new size
			Resize(max(index + toInsert.Size(), arraySize + toInsert.Size()));
			
			// Move data by the inserted array size
			if (arraySize) {
				for (kl::uint64 i = arraySize - 1; i >= index + toInsert.Size(); i--) {
					arrayData[i] = arrayData[i - toInsert.Size()];
				}
			}

			// Write inserted array data
			for (kl::uint64 i = 0; i < toInsert.Size(); i++) {
				arrayData[i + index] = toInsert[i];
			}
		}
		void Insert(kl::uint64 index, kl::array<T>&& toInsert) {
			Insert(index, toInsert);
		}

		// Fills the whole(or one part of) array with the given value
		void Fill(T value) {
			for (kl::uint64 i = 0; i < arraySize; i++) {
				arrayData[i] = value;
			}
		}
		void Fill(kl::uint64 startInclusive, kl::uint64 endExclusive, T value) {
			// Resizing if the end is bigger than array size
			if (endExclusive > arraySize) {
				Resize(endExclusive);
			}

			// Setting the data
			for (kl::uint64 i = startInclusive; i < endExclusive; i++) {
				arrayData[i] = value;
			}
		}

		// Returns the number of found elements
		kl::uint64 Count(T toCount) {
			kl::uint64 counter = 0;
			for (kl::uint64 i = 0; i < arraySize; i++) {
				if (arrayData[i] == toCount) {
					counter++;
				}
			}
			return counter;
		}

        // Returns the index of the first found element or -1 if the element was not found
		kl::int64 Find(T toFind) {
            for (kl::uint64 i = 0; i < arraySize; i++) {
                if (arrayData[i] == toFind) {
                    return (kl::int64)i;
                }
            }
            return -1;
        }

		// Returns an array of indexes of all found elements
		kl::array<kl::uint64> FindAll(T toFind) {
			kl::array<kl::uint64> indexArray;
			for (kl::uint64 i = 0; i < arraySize; i++) {
                if (arrayData[i] == toFind) {
                    indexArray <= i;
                }
            }
			return indexArray;
		}

        // Replaces all occurrences of an element with a given replace value
        // Returns the number of replaced elements
		kl::uint64 Replace(T toReplace, T with) {
			kl::uint64 replaceCounter = 0;
            for (kl::uint64 i = 0; i < arraySize; i++) {
                if (arrayData[i] == toReplace) {
                    arrayData[i] = with;
                    replaceCounter++;
                }
            }
            return replaceCounter;
        }

		// Executes a function on each array element
		void RunOnEach(std::function<void(T* elem)> toExecute) {
			for (kl::uint64 i = 0; i < arraySize; i++) {
				toExecute(arrayData + i);
			}
		}
		void RunOnEach(std::function<void(T* elem, int i)> toExecute) {
			for (kl::uint64 i = 0; i < arraySize; i++) {
				toExecute(arrayData + i, i);
			}
		}

	private:
		T* arrayData = nullptr;
		kl::uint64 arraySize = 0;
		bool canGrow = false;
	};
}
