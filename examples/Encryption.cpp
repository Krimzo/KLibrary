#include "KrimzLib.h"


int main()
{
	kl::encrypter encrypter;

	kl::mat4 data = kl::mat4::scaling({ 1.0f, 2.0f, 3.0f });
	kl::print(kl::colors::sky, data);

	encrypter.encrypt(data);
	kl::print(kl::colors::orange, data);

	encrypter.decrypt(data);
	kl::print(kl::colors::cyan, data);


	std::cin.get();
}
