#include "KrimzLib.h"


int main() {
	kl::Encrypter encrypter = {};

	kl::Mat4 data = kl::Mat4::Scaling({ 1.0f, 2.0f, 3.0f });
	kl::Print(kl::Colors::Sky, data);

	encrypter.encrypt(data);
	kl::Print(kl::Colors::Orange, data);

	encrypter.decrypt(data);
	kl::Print(kl::Colors::Cyan, data);


	kl::Get();
}
