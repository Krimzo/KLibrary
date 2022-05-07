#include "KrimzLib.h"


int main() {
	std::cout << kl::random::COLOR() << "hello there" << std::endl;

	std::cout << kl::random::COLOR() << kl::int2(1, 0) << std::endl;
	std::cout << kl::random::COLOR() << kl::int3(0, 1, 0) << std::endl;
	std::cout << kl::random::COLOR() << kl::int4(0, 0, 1, 0) << std::endl;

	std::cout << kl::random::COLOR() << kl::float2(1.0f, 0.0f) << std::endl;
	std::cout << kl::random::COLOR() << kl::float3(0.0f, 1.0f, 0.0f) << std::endl;
	std::cout << kl::random::COLOR() << kl::float4(0.0f, 0.0f, 1.0f, 0.0f) << std::endl;

	std::cout << kl::random::COLOR() << kl::mat3() << std::endl;
	std::cout << kl::random::COLOR() << kl::mat4() << std::endl;


	std::cin.get();
}