#include "KrimzLib.hpp"


int main() {
	kl::stack<int> pecanje;

	for (int i = 0; i < 10; i++) {
		pecanje.push(i);
	}

	kl::uint64 stackSize = 0;
	for (int i = 0; i < 10; i++) {
		std::cout << "value: " << pecanje.pop(&stackSize) << ", size: " << stackSize << "\n";
	}

	pecanje.push(42069);
	std::cout << "value: " << pecanje.pop() << ", size: " << stackSize << "\n";

	kl::console::waitFor(' ');
	return 0;
}