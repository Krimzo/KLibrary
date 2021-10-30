#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<kl::array<int>> array1(3);
    array1.ExecuteOnAll([](kl::array<int>& element) {
        element.SetSize(3);
        element.ExecuteOnAll([](int& element2) {
            element2 = 6;
        });
    });

    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            printf("%d ", array1[i][j]);
        }
        printf("\n");
    }


    printf("\n");
    return 0;
}