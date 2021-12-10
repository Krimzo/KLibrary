#include "KrimzLib.hpp"


int main() {
    kl::array<double> a(5);
    a.runOnEach([](double* elem) {
        *elem = kl::random::getInt(0, 10);
    });

    kl::array<double> b(5);
    b.runOnEach([](double* elem) {
        *elem = kl::random::getInt(0, 10);
    });

    kl::array<double> sum(5);
    sum.runOnEach([&](double* elem, int i) {
        *elem = a[i] + b[i];
    });
    
    for (int i = 0; i < sum.Size(); i++) {
        std::cout << sum[i] << '\n';
    }


	kl::console::waitFor(' ');
	return 0;
}