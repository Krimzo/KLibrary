#include "KrimzLib.h"


int main() {
    kl::image testImage(kl::size(800, 800));
    testImage.PerlinNoise(10);
    
    kl::window testWindow;
    testWindow.WindowUpdate = [&]() {
        testWindow.DisplayImage(testImage);
    };
    testWindow.StartNew({ 800, 800 }, "Test Window", false);


    //kl::console::WaitFor(' ');
    return 0;
}
