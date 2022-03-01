#include "KrimzLib/KrimzLib.hpp"



int main() {
	kl::window win;

	win.update = [&]() {
		static int frame = 0;
		std::cout << "Frame " << ++frame << std::endl;
	};

	std::thread([&]() {
		win.startNew(kl::ivec2(1600, 900), "xD", false, true);
	}).detach();


	std::cout << "Finished!\n";
	std::cin.get();
}

