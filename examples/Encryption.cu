#include "KrimzLib.h"


int main() {
	kl::encrypter enc;

	std::cout << kl::colors::wheat << enc;

	std::string message = "This is a test!";
	std::cout << kl::colors::sky << message << std::endl;

	enc.encrypt(&message[0], message.size());
	std::cout << kl::colors::orange << message << std::endl;

	enc.decrypt(&message[0], message.size());
	std::cout << kl::colors::cyan << message << std::endl;


	std::cin.get();
}
