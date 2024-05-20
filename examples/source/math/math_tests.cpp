#include "examples.h"


int examples::math_tests_main()
{
	const kl::Float3 a = { -1.0f, 0.3f, 2.1f };
	const kl::Float3 b = { 3.14f, 2.0f, -5.0f };

	kl::print("a = ", a);
	kl::print("b = ", b);
	kl::print("a + b = ", a + b);
	kl::print("a - b = ", a - b);
	kl::print("a * b = ", a * b);
	kl::print("a / b = ", a / b);
	kl::print("dot(a, b) = ", kl::dot(a, b));
	kl::print("cross(a, b) = ", kl::cross(a, b));
	kl::print("min(a, b) = ", kl::min(a, b));
	kl::print("max(a, b) = ", kl::max(a, b));
	kl::print("abs(a) = ", kl::abs(a));

	return 0;
}
