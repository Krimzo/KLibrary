#include "KrimzLib.hpp"


int main() {
    kl::array<float> a(5);
    a.runOnEach([](float* elem) {
        *elem = kl::random::getInt(0, 10);
    });

    kl::array<float> b(5);
    b.runOnEach([](float* elem) {
        *elem = kl::random::getInt(0, 10);
    });

    kl::array<float> sum(5);
    sum.runOnEach([&](float* elem, int i) {
        *elem = a[i] + b[i];
    });
    
    for (int i = 0; i < sum.Size(); i++) {
        std::cout << sum[i] << '\n';
    }


	kl::console::waitFor(' ');
	return 0;
}