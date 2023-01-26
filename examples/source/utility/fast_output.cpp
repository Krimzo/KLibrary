#include "klib.h"


static constexpr int N = 10000;

int main()
{
	float times[3] = {};

	kl::console::clear();
	kl::time::get_interval();
	for (int i = 0; i < N; i++) {
		printf("%d\n", i);
	}
	times[0] = kl::time::get_interval();

	kl::console::clear();
	kl::time::get_interval();
	for (int i = 0; i < N; i++) {
		std::cout << i << '\n';
	}
	times[1] = kl::time::get_interval();

	kl::console::clear();
	kl::time::get_interval();
	for (int i = 0; i < N; i++) {
		kl::print(i);
	}
	times[2] = kl::time::get_interval();

	print(kl::colors::yellow, "printf time: ", times[0]);
	print(kl::colors::cyan, "std::cout time: ", times[1]);
	print(kl::colors::orange, "kl::print time: ", times[2]);

	std::cin.get();
}
