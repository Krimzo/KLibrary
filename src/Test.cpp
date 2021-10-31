#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> testArray;

    testArray = { 1, 2, 3, 5 };



    for(int i=0; i<testArray.Size(); i++) {
        std::cout << testArray[i] << std::endl;
    }


    printf("\n");
    return 0;
}
