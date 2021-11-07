#pragma once


namespace kl {
	class string {
	public:
		// Returns the file extension from the given file path
		static std::string GetFileExtension(std::string filePath) {
			uint64 lastDotPos = filePath.find_last_of(".");
			if(lastDotPos == std::string::npos) {
				return "";
			}
  			return filePath.substr(lastDotPos + 1L);
		}
	};
}
