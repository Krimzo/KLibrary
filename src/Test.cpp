#include "KrimzLib.h"


int main() {
    kl::image testImage(kl::size(800, 800));
    testImage.RunOnEach([](kl::color* pixel, kl::point pos) {
        byte b = kl::random::Byte();
        pixel->r = b;
        pixel->g = b;
        pixel->b = b;
    });
    
    kl::window testWindow;
    testWindow.WindowUpdate = [&]() {
        testWindow.DisplayImage(testImage);
    };
    testWindow.StartNew({ 800, 800 }, "Test Window", false);


    //kl::console::WaitFor(' ');
    return 0;
}
