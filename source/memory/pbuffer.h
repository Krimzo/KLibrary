#pragma once

#include <iostream>
#include <vector>


namespace kl
{
	template<typename T> class pbuffer
	{
	private:
		std::vector<T*> instances;

	public:
		// Constructor
		pbuffer()
		{
		}

		// Destructor
		~pbuffer()
		{
			clear();
		}

		// Finds if pointer exists
		bool find(const T* inst, int* outInd = nullptr) const
		{
			for (int i = 0; i < instances.size(); i++)
			{
				if (instances[i] == inst)
				{
					if (outInd) *outInd = i;
					return true;
				}
			}
			return false;
		}

		// Creates a new instance
		T* newInst(T* inst)
		{
			instances.push_back(inst);
			return inst;
		}

		// Deletes the given instance
		bool delInst(const T* inst)
		{
			int ind = -1;
			if (find(inst, &ind))
			{
				delete instances[ind];
				instances.erase(instances.begin() + ind);
				return true;
			}
			return false;
		}

		// Returns the size
		size_t size() const
		{
			return instances.size();
		}

		// Operator overload
		T* operator[](const size_t i) const
		{
			return instances[i];
		}

		// Clears the buffer
		void clear()
		{
			for (int i = 0; i < instances.size(); i++)
				delete instances[i];
			instances.clear();
		}
	};
}
