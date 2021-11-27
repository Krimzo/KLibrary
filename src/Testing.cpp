#include "KrimzLib.h"


int main() {
    kl::array<double> a(5);
    a.RunOnEach([](double* elem) {
        *elem = kl::random::Int(0, 10);
    });

    kl::array<double> b(5);
    b.RunOnEach([](double* elem) {
        *elem = kl::random::Int(0, 10);
    });

    kl::array<double> sum(5);
    sum.RunOnEach([&](double* elem, int i) {
        *elem = a[i] + b[i];
    });
    
    for (int i = 0; i < sum.Size(); i++) {
        std::cout << sum[i] << '\n';
    }


	kl::console::WaitFor(' ');
	return 0;
}