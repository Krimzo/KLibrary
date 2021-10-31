#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> array1;

    array1 <= 25;
    array1 <= 6;
    array1 <= 15;


    for(int i=0; i<array1.GetSize(); i++) {
        std::cout << array1[i] << std::endl;
    }


    printf("\n");
    return 0;
}
