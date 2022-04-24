#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "libspec/using.h"
#include "geometry/vertex.h"


namespace kl {
	namespace file {
		// Returns the file extension from the given file path
		String getExtension(const String& filePath);

		// Returns the files in the given directory
		std::vector<String> getFiles(const String& dirPath, bool recursive = false);

		// Returns a string from a given text file
		String read(const String& filePath);
		std::vector<byte> readB(const String& filePath);

		// Writes text to a text file
		bool write(const String& filePath, const String& data);
		bool writeB(const String& filePath, const std::vector<byte>& data);

		// Appends text to a text file
		bool append(const String& filePath, const String& data, int position = -1);
		bool appendB(const String& filePath, const std::vector<byte>& data, int position = -1);

		// Parses given .obj file
		std::vector<kl::vertex> parseObj(const String& filePath, bool flipZ = true);
	};
}
