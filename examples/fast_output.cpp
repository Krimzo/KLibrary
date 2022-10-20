#include "KrimzLib.h"


static constexpr int N = 10000;

int main() {
	double times[3] = {};

	kl::Console::Clear();
	kl::Time::GetInterval();
	for (int i = 0; i < N; i++) {
		printf("%d\n", i);
	}
	times[0] = kl::Time::GetInterval();

	kl::Console::Clear();
	kl::Time::GetInterval();
	for (int i = 0; i < N; i++) {
		std::cout << i << '\n';
	}
	times[1] = kl::Time::GetInterval();

	kl::Console::Clear();
	kl::Time::GetInterval();
	for (int i = 0; i < N; i++) {
		kl::Print(i);
	}
	times[2] = kl::Time::GetInterval();

	kl::Print(kl::Colors::Yellow, "printf time: ", times[0]);
	kl::Print(kl::Colors::Cyan, "std::cout time: ", times[1]);
	kl::Print(kl::Colors::Orange, "kl::print time: ", times[2]);


	std::cin.get();
}
