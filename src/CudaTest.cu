#include "KrimzLib.hpp"


// Function declarations
__global__ void gpuInterpolation(kl::color*, kl::size, kl::triangle);

// Main func
int main() {
    // Frame buffer generation
    kl::image cpuFrameBuffer(kl::size(1600, 900));
    kl::gpubuff<kl::color> gpuFrameBuffer(cpuFrameBuffer.getPixelCount());

    // Triangle creation
    kl::triangle testTriangle;
    testTriangle.vertices[0].world = kl::vec3(100, cpuFrameBuffer.getHeight() - 100, 0);
    testTriangle.vertices[0].color = kl::color(255, 0, 0);
    testTriangle.vertices[1].world = kl::vec3(cpuFrameBuffer.getWidth() / 2, 100, 0);
    testTriangle.vertices[1].color = kl::color(0, 255, 0);
    testTriangle.vertices[2].world = kl::vec3(cpuFrameBuffer.getWidth() - 100, cpuFrameBuffer.getHeight() - 100, 0);
    testTriangle.vertices[2].color = kl::color(0, 0, 255);

    // Triangle rendering
    kl::time::staticGetElapsed();
    gpuInterpolation<<<cpuFrameBuffer.getPixelCount() / 256 + 1, 256>>>(gpuFrameBuffer.getPointer(), cpuFrameBuffer.getSize(), testTriangle);
    cudaDeviceSynchronize();
    gpuFrameBuffer.toRAM(cpuFrameBuffer.getPointer());
    std::cout << "Gpu elapsed time: " << kl::time::staticGetElapsed() << std::endl;

    // Window start function definition
    kl::window testWindow;
    testWindow.windowStart = [&]() {
        testWindow.renderImage(cpuFrameBuffer);
    };

    // Window creation
    testWindow.startNew(cpuFrameBuffer.getSize(), "Test", false);
    return 0;
}

// Triangle interpolation kernel
__global__ void gpuInterpolation(kl::color* frameBuffer, kl::size frameSize, kl::triangle tr) {
    // Getting the thread index
    const int i = blockIdx.x * blockDim.x + threadIdx.x;

    // Checking if the i is inside the frame
    if (i < frameSize.width * frameSize.height) {
        // Getting the x and y of the point
        const int x = i % frameSize.width;
        const int y = i / frameSize.width;

        // Calculating the interpolation constants
        float tempConst = 1 / ((tr.vertices[1].world.y - tr.vertices[2].world.y) * (tr.vertices[0].world.x - tr.vertices[2].world.x) + (tr.vertices[2].world.x - tr.vertices[1].world.x) * (tr.vertices[0].world.y - tr.vertices[2].world.y));
        float w1xConst = (tr.vertices[1].world.y - tr.vertices[2].world.y) * tempConst;
        float w1yConst = (tr.vertices[2].world.x - tr.vertices[1].world.x) * tempConst;
        float w2xConst = (tr.vertices[2].world.y - tr.vertices[0].world.y) * tempConst;
        float w2yConst = (tr.vertices[0].world.x - tr.vertices[2].world.x) * tempConst;

        // Calculating the interpolation weights
        float interWeight1 = (x - tr.vertices[2].world.x) * w1xConst + (y - tr.vertices[2].world.y) * w1yConst;
        float interWeight2 = (x - tr.vertices[2].world.x) * w2xConst + (y - tr.vertices[2].world.y) * w2yConst;
        float interWeight3 = 1 - interWeight1 - interWeight2;

        // Checking if the point is inside the triangle
        if (!(interWeight1 < 0 || interWeight2 < 0 || interWeight3 < 0)) {
            // Interpolating the color
            frameBuffer[i].r = kl::byte(tr.vertices[0].color.r * interWeight1 + tr.vertices[1].color.r * interWeight2 + tr.vertices[2].color.r * interWeight3);
            frameBuffer[i].g = kl::byte(tr.vertices[0].color.g * interWeight1 + tr.vertices[1].color.g * interWeight2 + tr.vertices[2].color.g * interWeight3);
            frameBuffer[i].b = kl::byte(tr.vertices[0].color.b * interWeight1 + tr.vertices[1].color.b * interWeight2 + tr.vertices[2].color.b * interWeight3);
        }
        else {
            // Drawing the background
            frameBuffer[i].r = 50;
            frameBuffer[i].g = 50;
            frameBuffer[i].b = 50;
        }
    }
}
