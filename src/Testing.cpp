#include "KrimzLib.hpp"


int main() {
	kl::array<kl::array<std::string>> a;

	for (int i = 0; i < 7; i++) {
		a <= kl::array<std::string>();
		for (int j = 0; j < 3; j++) {
			a[i] <= kl::random::getString(kl::random::getInt(1, 13));
		}
	}

	a.forEach([](auto* elem) {
		elem->forEach([](auto* str) {
			kl::console::println(*str, kl::random::getColor());
		});
	});


	kl::console::waitFor(' ');
	return 0;
}