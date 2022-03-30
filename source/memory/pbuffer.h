#pragma once

#include <iostream>
#include <vector>


namespace kl {
	template<typename T> class pbuffer {
	private:
		std::vector<T*> instances;

	public:
		// Constructor
		pbuffer() {

		}

		// Destructor
		~pbuffer() {
			this->clear();
		}

		// Creates a new instance
		T* newInst(T* inst) {
			instances.push_back(inst);
			return inst;
		}

		// Deletes the given instance
		bool delInst(T* inst) {
			for (int i = 0; i < instances.size(); i++) {
				if (instances[i] == inst) {
					delete instances[i];
					instances.erase(instances.begin() + i);
					return true;
				}
			}
			return false;
		}

		// Returns the size
		size_t size() {
			return instances.size();
		}

		// Operator overload
		T* operator[](const size_t i) {
			return instances[i];
		}

		// Clears the buffer
		void clear() {
			for (int i = 0; i < instances.size(); i++) {
				delete instances[i];
			}
			instances.clear();
		}
	};
}
