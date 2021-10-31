#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> testArray(10);

    testArray.RunOnAll([](int& elem) {
        elem = kl::random::Int(0, 10);
    });

    kl::array<int> b = testArray;

    for(int i=0; i<b.Size(); i++) {
        std::cout << b[i] << std::endl;
    }


    printf("\n");
    return 0;
}
