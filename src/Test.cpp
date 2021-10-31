#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> array1;
    array1.Fill(6);

    printf("%p\n", array1.GetRawData());

    array1.SetSize(5);

    printf("%p\n", array1.GetRawData());

    for(int i=0; i<array1.GetSize(); i++) {
        printf("%d\n", array1[i]);
    }


    printf("\n");
    return 0;
}
