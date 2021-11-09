#include "KrimzLib.h"


int main() {
    // Const
    const kl::size customSize = { 900, 900 };

    // Window and image
    kl::window testWindow;
    kl::image testImage(customSize);

    // User variables
    kl::point A = { 100, 100 };
    kl::point B = { 800, 800 };
    kl::point* toMove = NULL;

    int grabDistance = 50;
    testWindow.WindowUpdate = [&]() {
        // Image clearing
        testImage.FillSolid(kl::constant::colors::gray);

        // User editing
        double distanceFromA = kl::vec2(A, testWindow.MOUSE.position).Length();
        double distanceFromB = kl::vec2(B, testWindow.MOUSE.position).Length();
 
        if (distanceFromA < distanceFromB) {
            if (distanceFromA < grabDistance) {
                toMove = &A;
            }
            else {
                toMove = NULL;
            }
        }
        else {
            if (distanceFromB < grabDistance) {
                toMove = &B;
            }
            else {
                toMove = NULL;
            }
        }

        if (toMove) {
            if (testWindow.MOUSE.lmb) {
                *toMove = testWindow.MOUSE.position;
            }

            testImage.DrawCircle(*toMove, testWindow.MOUSE.position, kl::constant::colors::cyan);
        }

        testImage.DrawTriangle(A, B, testWindow.MOUSE.position, kl::constant::colors::wheat);
        testImage.DrawLine(A, B, kl::constant::colors::orange);

        // Image displaying
        testWindow.DisplayImage(testImage);
    };

    testWindow.StartNew(customSize, "Test Window", false);


    //kl::console::WaitFor(' ');
    return 0;
}
