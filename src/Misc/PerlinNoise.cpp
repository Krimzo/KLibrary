#include "KrimzLib.hpp"


int main() {
    kl::image testImage(kl::ivec2(800, 800));
    testImage.genPerlinNoise(10);
    
    kl::window testWindow;
    testWindow.start = [&]() {
        testWindow.drawImage(testImage);
    };
    testWindow.startNew(testImage.getSize(), "Test Window", false);


    return 0;
}
