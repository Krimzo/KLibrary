#include "KrimzLib.h"


const double timePerTest = 2;

int main() {
    // prep
    const int frameWidth = kl::console::GetSize().width;
    const int frameHeight = kl::console::GetSize().height - 1;
    const int charCount = frameWidth * frameHeight;
    std::string frameBuffer;
    frameBuffer.resize(charCount);
    for (int i = 0; i < charCount; i++) {
        frameBuffer[i] = '#';
    }
    kl::console::HideCursor();
    
    // printf
    int printfCounter = 0;
    kl::time::StaticStopwatchReset();
    while (kl::time::StaticStopwatchElapsed() < timePerTest) {
        kl::console::MoveCursor({ 0, 0 });
        printf("%s", frameBuffer.c_str());
        printfCounter++;
    }

    // stdout
    int stdoutCounter = 0;
    kl::time::StaticStopwatchReset();
    while (kl::time::StaticStopwatchElapsed() < timePerTest) {
        kl::console::MoveCursor({ 0, 0 });
        std::cout << frameBuffer;
        stdoutCounter++;
    }

    // fwrite
    int fwriteCounter = 0;
    kl::time::StaticStopwatchReset();
    while (kl::time::StaticStopwatchElapsed() < timePerTest) {
        kl::console::MoveCursor({ 0, 0 });
        fwrite(frameBuffer.c_str(), 1, charCount, stdout);
        fwriteCounter++;
    }
    
    // myfout
    int myfoutCounter = 0;
    kl::time::StaticStopwatchReset();
    while (kl::time::StaticStopwatchElapsed() < timePerTest) {
        kl::console::FastOut(frameBuffer);
        myfoutCounter++;
    }

    // printing results
    kl::console::Clear();
    printf("printf => %d fps\n", int(printfCounter / timePerTest));
    printf("stdout => %d fps\n", int(stdoutCounter / timePerTest));
    printf("fwrite => %d fps\n", int(fwriteCounter / timePerTest));
    printf("myfout => %d fps\n", int(myfoutCounter / timePerTest));


    kl::console::WaitFor(' ');
    return 0;
}
