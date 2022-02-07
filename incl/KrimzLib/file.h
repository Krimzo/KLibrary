#pragma once

#include <string>


namespace kl {
	namespace file {
		// Returns the file extension from the given file path
		std::string getExtension(const std::string& filePath);

		// Returns a string from a given text file
		std::string read(const std::string& filePath);

		// Writes text to a text file
		void write(const std::string& filePath, const std::string& data);

		// Appends text to a text file
		void append(const std::string& filePath, const std::string& data, int position = -1);
	};
}
