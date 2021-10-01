__kernel void TestKernel(__global double* givenArray) {
    int i = get_global_id(0);

    givenArray[i] = sin((double)i) / sqrt((double)i);
}