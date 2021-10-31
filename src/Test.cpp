#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> array1 = { 12 };


    for(int i=0; i<array1.GetSize(); i++) {
        std::cout << array1[i] << std::endl;
    }


    printf("\n");
    return 0;
}
