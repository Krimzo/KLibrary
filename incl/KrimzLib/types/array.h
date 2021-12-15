#pragma once


namespace kl {
	template<typename T> struct array {
		// Constructors and destructor
		array(kl::uint64 arraySize = 0, bool canGrow = false) {
			// Update properties
			this->canGrow = canGrow;

			// Resize
			resize(arraySize);
		}
		array(kl::array<T>& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.growth();

			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		array(kl::array<T>&& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.growth();

			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		array(std::initializer_list<T> listOfElements) {
			// Resize
			resize(listOfElements.size());

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
					exit(69);
				}

				// Resize if the growth is enabled
				resize(index + 1);
			}

			// Return element
			return arrayData[index];
		}
		void operator = (kl::array<T>& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.growth();

			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		void operator = (kl::array<T>&& arrayToCopy) {
			// Update properties
			canGrow = arrayToCopy.growth();

			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (kl::uint64 i = 0; i < arrayToCopy.size(); i++) {
				arrayData = arrayToCopy[i];
			}
		}
		void operator = (std::initializer_list<T> listOfElements) {
			// Resize
			resize(listOfElements.size());

			// Copy data from the initialiser list to the array memory
			for (kl::uint64 i = 0; i < listOfElements.size(); i++) {
				arrayData[i] = listOfElements.begin()[i];
			}
		}
		kl::uint64 operator ++ (int ignore) {
			resize(arraySize + 1);
			return arraySize;
		}
		kl::uint64 operator -- (int ignore) {
			if (arraySize) {
				resize(arraySize - 1);
				return arraySize;
			}
			printf("Error, can't shrink array of size 0!\n");
			exit(69);
		}
		void operator <= (T toAdd) {
			resize(arraySize + 1);
			back() = toAdd;
		}

		// Returns the current array size
		kl::uint64 size() {
			return arraySize;
		}

		// Changes the array size
		void resize(kl::uint64 newSize) {
			// Allocate temp memory
			T* tempMemory = new T[newSize];
			if (!tempMemory) {
				printf("Couldn't allocate %llu bytes of memory..\n", newSize * sizeof(T));
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
		void clear() {
			resize(0);
		}

		// Returns the first element
		T& front() {
			if (arraySize) {
				return arrayData[0];
			}
			printf("Error. Trying to access memory outside the non growing array\n");
			exit(69);
		}

		// Returns the last element
		T& back() {
			if (arraySize) {
				return arrayData[arraySize - 1];
			}
			printf("Error. Trying to access memory outside the non growing array\n");
			exit(69);
		}

		// Returns the pointer to the raw data
		T* getPointer() {
			return arrayData;
		}

		// Returns true if the array has growth enabled or false if not
		bool growth() {
			return canGrow;
		}
		void growth(bool canGrow) {
			this->canGrow = canGrow;
		}

		// Inserts an array of elements on the given index
		void insert(kl::uint64 index, kl::array<T>& toInsert) {
			// Update to fit the new size
			resize(max(index + toInsert.size(), arraySize + toInsert.size()));
			
			// Move data by the inserted array size
			if (arraySize) {
				for (kl::uint64 i = arraySize - 1; i >= index + toInsert.size(); i--) {
					arrayData[i] = arrayData[i - toInsert.size()];
				}
			}

			// Write inserted array data
			for (kl::uint64 i = 0; i < toInsert.size(); i++) {
				arrayData[i + index] = toInsert[i];
			}
		}
		void insert(kl::uint64 index, kl::array<T>&& toInsert) {
			insert(index, toInsert);
		}

		// Fills the whole(or one part of) array with the given value
		void fill(T value) {
			for (kl::uint64 i = 0; i < arraySize; i++) {
				arrayData[i] = value;
			}
		}
		void fill(kl::uint64 startInclusive, kl::uint64 endExclusive, T value) {
			// Resizing if the end is bigger than array size
			if (endExclusive > arraySize) {
				resize(endExclusive);
			}

			// Setting the data
			for (kl::uint64 i = startInclusive; i < endExclusive; i++) {
				arrayData[i] = value;
			}
		}

		// Returns the number of found elements
		kl::uint64 count(T toCount) {
			kl::uint64 counter = 0;
			for (kl::uint64 i = 0; i < arraySize; i++) {
				if (arrayData[i] == toCount) {
					counter++;
				}
			}
			return counter;
		}

        // Returns the index of the first found element or -1 if the element was not found
		kl::int64 find(T toFind) {
            for (kl::uint64 i = 0; i < arraySize; i++) {
                if (arrayData[i] == toFind) {
                    return (kl::int64)i;
                }
            }
            return -1;
        }

		// Returns an array of indexes of all found elements
		kl::array<kl::uint64> findAll(T toFind) {
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
		kl::uint64 replace(T toReplace, T with) {
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
		void runOnEach(std::function<void(T* elem)> toExecute) {
			for (kl::uint64 i = 0; i < arraySize; i++) {
				toExecute(arrayData + i);
			}
		}
		void runOnEach(std::function<void(T* elem, int i)> toExecute) {
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
