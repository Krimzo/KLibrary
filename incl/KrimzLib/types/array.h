#pragma once


namespace kl {
	template<typename T> struct array {
		// Constructors and destructor
		array(int arraySize = 0) {
			resize(arraySize);
		}
		array(kl::array<T>& arrayToCopy) {
			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (int i = 0; i < arrayToCopy.size(); i++) {
				arrayData[i] = arrayToCopy[i];
			}
		}
		array(kl::array<T>&& arrayToCopy) {
			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (int i = 0; i < arrayToCopy.size(); i++) {
				arrayData[i] = arrayToCopy[i];
			}
		}
		array(std::initializer_list<T> listOfElements) {
			// Resize
			resize(listOfElements.size());

			// Copy data from the initialiser list to the array memory
			for (int i = 0; i < listOfElements.size(); i++) {
				arrayData[i] = listOfElements.begin()[i];
			}
		}
		~array() {
			delete[] arrayData;
		}

		// Operator overloading
		T& operator [] (int index) {
			// Check if the index is out of the array bounds
			if (index > (arraySize - 1)) {
				printf("Error. Trying to access memory outside the array\n");
				exit(69);
			}

			// Return element
			return arrayData[index];
		}
		void operator = (kl::array<T>& arrayToCopy) {
			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (int i = 0; i < arrayToCopy.size(); i++) {
				arrayData[i] = arrayToCopy[i];
			}
		}
		void operator = (kl::array<T>&& arrayToCopy) {
			// Resize
			resize(arrayToCopy.size());

			// Copy the data
			for (int i = 0; i < arrayToCopy.size(); i++) {
				arrayData[i] = arrayToCopy[i];
			}
		}
		void operator = (std::initializer_list<T> listOfElements) {
			// Resize
			resize(listOfElements.size());

			// Copy data from the initialiser list to the array memory
			for (int i = 0; i < listOfElements.size(); i++) {
				arrayData[i] = listOfElements.begin()[i];
			}
		}
		int operator ++ (int ignore) {
			resize(arraySize + 1);
			return arraySize;
		}
		int operator -- (int ignore) {
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
		int size() {
			return arraySize;
		}

		// Changes the array size
		void resize(int newSize) {
			// Allocating temp memory
			T* tempMemory = new T[newSize];
			if (!tempMemory) {
				printf("Couldn't allocate %d bytes of memory..\n", newSize * sizeof(T));
                exit(69);
			}

			// Setting newly allocated addresses to the default values
			if (newSize > arraySize) {
				for (int i = arraySize; i < newSize; i++) {
					tempMemory[i] = {};
				}
			}

			// Copying the data from the old to the temp memory
			int minSize = std::min(arraySize, newSize);
			for (int i = 0; i < minSize; i++) {
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
			printf("Error. Trying to access memory outside the array\n");
			exit(69);
		}

		// Returns the last element
		T& back() {
			if (arraySize) {
				return arrayData[arraySize - 1];
			}
			printf("Error. Trying to access memory outside the array\n");
			exit(69);
		}

		// Returns the pointer to the raw data
		T* pointer() {
			return arrayData;
		}

		// Inserts an array of elements on the given index
		void insert(int index, kl::array<T>& toInsert) {
			// Update to fit the new size
			resize(std::max(index + toInsert.size(), arraySize + toInsert.size()));
			
			// Move data by the inserted array size
			if (arraySize) {
				for (int i = arraySize - 1; i >= index + toInsert.size(); i--) {
					arrayData[i] = arrayData[i - toInsert.size()];
				}
			}

			// Write inserted array data
			for (int i = 0; i < toInsert.size(); i++) {
				arrayData[i + index] = toInsert[i];
			}
		}
		void insert(int index, kl::array<T>&& toInsert) {
			insert(index, toInsert);
		}

		// Fills the whole(or one part of) array with the given value
		void fill(T value) {
			for (int i = 0; i < arraySize; i++) {
				arrayData[i] = value;
			}
		}
		void fill(int startInclusive, int endExclusive, T value) {
			// Resizing if the end is bigger than array size
			if (endExclusive > arraySize) {
				resize(endExclusive);
			}

			// Setting the data
			for (int i = startInclusive; i < endExclusive; i++) {
				arrayData[i] = value;
			}
		}

		// Returns the number of found elements
		int count(T toCount) {
			int counter = 0;
			for (int i = 0; i < arraySize; i++) {
				if (arrayData[i] == toCount) {
					counter++;
				}
			}
			return counter;
		}

        // Returns the index of the first found element or -1 if the element was not found
		int find(T toFind) {
            for (int i = 0; i < arraySize; i++) {
                if (arrayData[i] == toFind) {
                    return i;
                }
            }
            return -1;
        }

		// Returns an array of indexes of all found elements
		kl::array<int> findAll(T toFind) {
			kl::array<int> indexArray;
			for (int i = 0; i < arraySize; i++) {
                if (arrayData[i] == toFind) {
                    indexArray <= i;
                }
            }
			return indexArray;
		}

        // Replaces all occurrences of an element with a given replace value
        // Returns the number of replaced elements
		int replace(T toReplace, T with) {
			int replaced = 0;
            for (int i = 0; i < arraySize; i++) {
                if (arrayData[i] == toReplace) {
                    arrayData[i] = with;
                    replaced++;
                }
            }
            return replaced;
        }

		// Executes a function on each array element
		void forEach(std::function<void(T* elem)> toExecute) {
			for (int i = 0; i < arraySize; i++) {
				toExecute(arrayData + i);
			}
		}

	private:
		T* arrayData = nullptr;
		int arraySize = 0;
	};
}
