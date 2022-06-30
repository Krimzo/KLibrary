#include "KrimzLib.h"


int main() {
	kl::encrypter enc;

	kl::mat4 data = kl::mat4::scaling({ 1.0f, 2.0f, 3.0f });
	kl::print(kl::colors::sky, data);

	enc.encrypt(data);
	kl::print(kl::colors::orange, data);

	enc.decrypt(data);
	kl::print(kl::colors::cyan, data);


	std::cin.get();
}
