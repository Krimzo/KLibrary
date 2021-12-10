#pragma once


namespace kl {
    template<typename T> class gpubuff {
    public:
        // Constructor/Destructor
        gpubuff(uint64_t n) {
            typeSize = sizeof(T);
            allocateGpuMemory(n);
        }
        ~gpubuff() {
            freeGpuMemory();
        }

        // Returns the size of the array
        int getSize() {
            return size;
        }

        // Resizes the buffer
        void resize(uint64_t n, bool copyData = true) {
            if (copyData) {
                T* tempRamBuffer = new T[n];
                if (!tempRamBuffer) exit(-1);
                toRAM(tempRamBuffer);
                freeGpuMemory();
                allocateGpuMemory(n);
                fromRAM(tempRamBuffer);
                delete[] tempRamBuffer;
            }
            else {
                freeGpuMemory();
                allocateGpuMemory(n);
            }
        }

        // Returns the pointer of the buffer memory
        T* getPointer() {
            return memory;
        }

        // Copies the data from the host
        void fromRAM(T* hostBuff) {
            cudaMemcpy(memory, hostBuff, size * typeSize, cudaMemcpyHostToDevice);
        }
        void fromRAM(T* hostBuff, uint64_t n) {
            cudaMemcpy(memory, hostBuff, n * typeSize, cudaMemcpyHostToDevice);
        }

        // Copies the data to the host
        void toRAM(T* hostBuff) {
            cudaMemcpy(hostBuff, memory, size * typeSize, cudaMemcpyDeviceToHost);
        }
        void toRAM(T* hostBuff, uint64_t n) {
            cudaMemcpy(hostBuff, memory, n * typeSize, cudaMemcpyDeviceToHost);
        }

    private:
        // Data
        T* memory;
        int typeSize;
        uint64_t size;

        // Allocates the memory on the gpu
        void allocateGpuMemory(uint64_t n) {
            cudaMalloc(&memory, n * typeSize);
            validateGpuMemory(n);
            size = n;
        }

        // Checks if the memory has been allocated
        void validateGpuMemory(uint64_t n) {
            if (!memory) {
                printf("Could not allocate %llu bytes of the gpu memory!\nExiting..", n * typeSize);
                exit(-1);
            }
        }

        // Frees the allocated memory
        void freeGpuMemory() {
            cudaFree(memory);
            size = 0;
        }
    };
}
