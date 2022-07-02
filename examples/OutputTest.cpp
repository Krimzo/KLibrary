#include "KrimzLib.h"


constexpr int N = 10000;

int main() {
	double times[3] = {};

	kl::console::clear();
	kl::time::interval();
	for (int i = 0; i < N; i++) {
		printf("%d\n", i);
	}
	times[0] = kl::time::interval();

	kl::console::clear();
	kl::time::interval();
	for (int i = 0; i < N; i++) {
		std::cout << i << '\n';
	}
	times[1] = kl::time::interval();

	kl::console::clear();
	kl::time::interval();
	for (int i = 0; i < N; i++) {
		kl::print(i);
	}
	times[2] = kl::time::interval();

	kl::print(kl::colors::yellow, "printf time: ", times[0]);
	kl::print(kl::colors::cyan, "std::cout time: ", times[1]);
	kl::print(kl::colors::orange, "kl::print time: ", times[2]);


	std::cin.get();
}
