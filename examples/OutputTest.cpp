#include "KrimzLib.h"


constexpr int N = 10000;

int main() {
	kl::time::interval();
	for (int i = 0; i < N; i++) {
		printf("%d\n", i);
	}
	const double printfTime = kl::time::interval();
	for (int i = 0; i < N; i++) {
		std::cout << i << '\n';
	}
	const double coutTime = kl::time::interval();
	for (int i = 0; i < N; i++) {
		kl::print(i);
	}
	const double printTime = kl::time::interval();

	kl::print("printf time: ", printfTime);
	kl::print("std::cout time: ", coutTime);
	kl::print("kl::print time: ", printTime);


	std::cin.get();
}
