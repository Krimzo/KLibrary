#include "KrimzLib.hpp"


int main() {
	// Info
	const std::string dirs[] = { "incl", "src", "res/shaders" };
	const std::string blacklist[] = { "glad.h", "glad.cpp" };

	// Dir loop
	int lineCount = 0;
	for (const std::string& dir : dirs) {
		// Getting dir files
		std::vector<std::string> files = kl::file::getFiles(dir, true);

		// File loop
		for (const std::string& file : files) {
			// Blacklist loop
			bool badFile = false;
			for (const std::string& bad : blacklist) {
				if (file.find(bad) != std::string::npos) {
					badFile = true;
					break;
				}
			}

			// Bad file check
			if (!badFile) {
				// Opening the file
				std::ifstream in(file);

				// File check
				if (in.is_open()) {
					// Line loop
					for (std::string line; std::getline(in, line);) {
						lineCount++;
					}

					// Closing the file
					in.close();
				}
				else {
					std::cout << "Could not open file \"" << file << "\"!";
					std::cin.get();
					return 1;
				}
			}
		}
	}

	// Line out
	std::cout << lineCount << " lines!";
	std::cin.get();
}
