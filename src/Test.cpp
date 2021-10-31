#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> array1 = { 1, 3, 6, 7, 6, 12, 61, 13, 6, 2 };

    // insert
    

    for(int i=0; i<array1.GetSize(); i++) {
        std::cout << array1[i] << std::endl;
    }


    printf("\n");
    return 0;
}
