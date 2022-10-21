#include "klib.h"


int main()
{
	const kl::encryptor encryptor = {};

	kl::mat4 data = kl::mat4::scaling({ 1.0f, 2.0f, 3.0f });
	print(kl::colors::sky, data);

	encryptor.encrypt(data);
	print(kl::colors::orange, data);

	encryptor.decrypt(data);
	print(kl::colors::cyan, data);

	kl::get();
}
