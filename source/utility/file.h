#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "render/vertex.h"


namespace kl {
	namespace file {
		std::vector<std::string> getFiles(const std::string& dirPath, bool recursive = false);

		std::string extension(const std::string& filePath);

		std::string read(const std::string& filePath);
		std::vector<byte> readB(const std::string& filePath);

		bool write(const std::string& filePath, const std::string& data);
		uint64 writeB(const std::string& filePath, const std::vector<byte>& data);

		bool append(const std::string& filePath, const std::string& data, int position = -1);
		uint64 appendB(const std::string& filePath, const std::vector<byte>& data, int position = -1);

		std::vector<kl::vertex> parseMesh(const std::string& filePath, bool flipZ = true);
	};
}
