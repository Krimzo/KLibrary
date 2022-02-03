#include "KrimzLib.hpp"


void PrintSite(String url, float charsPerSecond = 100) {
	// Static data
	static const char charTests[] = { ';', '{', '}' };
	static const Function<bool(char c)> NewLineTest = [&](char c) {
		for (char t : charTests) {
			if (c == t) {
				return true;
			}
		}
		return false;
	};
	
	// Outputting
	const String data = kl::web::getWebsiteData(url);
	const float timeToSleep = 1.0f / charsPerSecond;
	kl::timer timer;
	for (char c : data) {
		// Output
		kl::console::print(c);

		// New line
		if (NewLineTest(c)) {
			_putch('\n');
		}

		// Sleep
		while (timer.elapsed() < timeToSleep);
		timer.reset();
	}
}

int main() {
	PrintSite("https://www.krimzo.eu/");


	std::cin.get();
}
