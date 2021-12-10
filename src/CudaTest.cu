#include "KrimzLib.hpp"


// Function declarations
void cpuInterpolation(kl::color*, kl::size, kl::triangle);
__global__ void gpuInterpolation(kl::color*, kl::size, kl::triangle);// cpu => 491401

// Main func
int main() {
    // Rendering device type
    const bool useGpu = true;

    // Ram frame buffer
    kl::image cpuFrameBuffer(kl::size(1600, 900));

    // Triangle creation
    kl::triangle testTriangle;
    testTriangle.vertices[0].world = kl::vec3(100, cpuFrameBuffer.getHeight() - 100, 0);
    testTriangle.vertices[0].color = kl::constant::colors::red;
    testTriangle.vertices[1].world = kl::vec3(cpuFrameBuffer.getWidth() / 2, 100, 0);
    testTriangle.vertices[1].color = kl::constant::colors::green;
    testTriangle.vertices[2].world = kl::vec3(cpuFrameBuffer.getWidth() - 100, cpuFrameBuffer.getHeight() - 100, 0);
    testTriangle.vertices[2].color = kl::constant::colors::blue;

    // Triangle rendering
    kl::time::staticGetElapsed();
    if (useGpu) {
        kl::gpubuff<kl::color> gpuFrameBuffer(cpuFrameBuffer.getPixelCount());
        gpuInterpolation<<<cpuFrameBuffer.getPixelCount() / 256 + 1, 256>>>(gpuFrameBuffer.getPointer(), cpuFrameBuffer.getSize(), testTriangle);
        gpuFrameBuffer.toRAM(cpuFrameBuffer.getPointer());
    }
    else {
        cpuInterpolation(cpuFrameBuffer.getPointer(), cpuFrameBuffer.getSize(), testTriangle);
    }
    std::cout << (useGpu ? "Gpu " : "Cpu ") << " elapsed time: " << kl::time::staticGetElapsed() << std::endl;

    // Window start function definition
    kl::window testWindow;
    testWindow.windowStart = [&]() {
        testWindow.renderImage(cpuFrameBuffer);
    };

    // Window creation
    testWindow.startNew(cpuFrameBuffer.getSize(), "Test", false);
    return 0;
}


// Triangle interpolation with cpu
void cpuInterpolation(kl::color* frameBuffer, kl::size frameSize, kl::triangle tr) {
    tr.computeInterConsts();
    for(int i = 0; i < frameSize.width * frameSize.height; i++) {
        // Getting the x and y of the point
        const int x = i % frameSize.width;
        const int y = i / frameSize.width;

        // Calculating the interpolation weights
        kl::vec3 weights = tr.getInterWeights(kl::vec2(x, y));

        // Checking if the point is inside the triangle
        if (tr.inTriangle(weights)) {
            // Interpolating the color
            frameBuffer[i] = tr.interpolateColor(weights);
        }
        else {
            // Drawing the background
            frameBuffer[i] = kl::constant::colors::gray;
        }
    }
}

struct vec2 {
    double x;
    double y;
};

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
        double tempConst = 1 / ((tr.vertices[1].world.y - tr.vertices[2].world.y) * (tr.vertices[0].world.x - tr.vertices[2].world.x) + (tr.vertices[2].world.x - tr.vertices[1].world.x) * (tr.vertices[0].world.y - tr.vertices[2].world.y));
        double w1xConst = (tr.vertices[1].world.y - tr.vertices[2].world.y) * tempConst;
        double w1yConst = (tr.vertices[2].world.x - tr.vertices[1].world.x) * tempConst;
        double w2xConst = (tr.vertices[2].world.y - tr.vertices[0].world.y) * tempConst;
        double w2yConst = (tr.vertices[0].world.x - tr.vertices[2].world.x) * tempConst;

        // Calculating the interpolation weights
        double interWeight1 = (x - tr.vertices[2].world.x) * w1xConst + (y - tr.vertices[2].world.y) * w1yConst;
        double interWeight2 = (x - tr.vertices[2].world.x) * w2xConst + (y - tr.vertices[2].world.y) * w2yConst;
        double interWeight3 = 1 - interWeight1 - interWeight2;

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
