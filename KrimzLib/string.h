#pragma once


namespace kl {
	class string {
	public:
		static std::wstring GetFileExtension(std::wstring filePath) {
			return PathFindExtensionW(filePath.c_str());
		}
		static std::string GetFileExtension(std::string filePath) {
			return PathFindExtensionA(filePath.c_str());
		}
	};
}
