#include "KrimzLib.h"


int main() {
	std::list<int> a;

	a.push_back(0);

	for (int i = 0; i < 25; i++) {
		printf("0x%p\n", &a.front());
		a.push_back(i);
	}


	kl::console::WaitFor(' ');
	return 0;
}