#include "KrimzLib.hpp"


int main() {
    kl::image testImage(kl::size(800, 800));
    testImage.genPerlinNoise(10);
    
    kl::window testWindow;
    testWindow.windowUpdate = [&]() {
        testWindow.DisplayImage(testImage);
    };
    testWindow.startNew({ 800, 800 }, "Test Window", false);


    return 0;
}
