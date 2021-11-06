#include "KrimzLib.h"


int main() {
    kl::InitLib();


    kl::array<int> a(6);

    a.Fill(kl::random::Int(0, 10));

    a.RunOnEach([](int& elem) {
        kl::console::Println(elem);
    });
    

    kl::console::WaitFor(' ', true);
    return 0;
}
