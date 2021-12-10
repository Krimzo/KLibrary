#pragma once


namespace kl {
	namespace string {
		// Returns the file extension from the given file path
		std::string getFileExtension(std::string filePath) {
			kl::uint64 lastDotPos = filePath.find_last_of(".");
			if(lastDotPos == std::string::npos) {
				return "";
			}
  			return filePath.substr(lastDotPos + 1L);
		}
	};
}
