#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> a(10);

    for (int i = 0; i < a.GetSize(); i++) {
        a[i] = kl::random::Int(0, 10);
    }

    kl::console::Println(a.Replace(6, 26), kl::constant::colors::cyan);

    for(int i=0; i<a.GetSize(); i++) {
        printf("%d => %d\n", i, a[i]);
    }


    return 0;
}