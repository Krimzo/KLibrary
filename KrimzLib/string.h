#pragma once


namespace kl {
	class string {
	public:
		// Returns the file extension from the given file path
		static std::wstring GetFileExtension(std::wstring filePath) {
			return PathFindExtensionW(filePath.c_str());
		}
		static std::string GetFileExtension(std::string filePath) {
			return PathFindExtensionA(filePath.c_str());
		}
	};
}
