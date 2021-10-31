#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> array1(10);
    array1.Fill(6);

    array1.SetSize(5);

    for(int i=0; i<array1.GetSize(); i++) {
        printf("%d\n", array1[i]);
    }


    printf("\n");
    return 0;
}
