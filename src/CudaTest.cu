#include "KrimzLib.h"


__global__ void testKernel(kl::vec2* dataArray) {
    const int i = blockIdx.x * blockDim.x + threadIdx.x;

    dataArray[i].x = i;
    dataArray[i].y = i * 3.14;
}

int main() {
    const int n = 6;

    kl::vec2* a = new kl::vec2[n];
    kl::gpubuff<kl::vec2> b(n);

    testKernel<<<1, n>>>(b.getPointer());
    cudaDeviceSynchronize();

    b.toRAM(a);

    for(int i = 0; i < n; i++) {
        a[i].print();
    }

    delete[] a;

    return 0;
}
