#pragma once


namespace kl {
	class file {
	public:
		// Creates a file
		static void Create(std::string filePath) {
			std::ofstream(filePath).close();
		}

		// Opens a file with default program
		static void OpenWithDefault(std::string filePath) {
			system(filePath.c_str());
		}

		// Opens a file with notepad
		static void OpenWithNotepad(std::string filePath) {
			system(("notepad " + filePath).c_str());
		}

		// Opens a file with visual studio code
		static void OpenWithVSCode(std::string filePath) {
			system(("code " + filePath).c_str());
		}

		// Returns a string from a given text file
		static std::string ReadText(std::string filePath) {
			std::ifstream fileStream(filePath);
			std::stringstream textBuffer;
			if (!fileStream.is_open()) {
				printf("Couldn't load text file \"%s\".", filePath.c_str());
				console::WaitFor(' ', true);
				exit(69);
			}
			textBuffer << fileStream.rdbuf();
			fileStream.close();
			return textBuffer.str();
		}

		// Writes text to a text file
		static void WriteText(std::string& data, std::string filePath) {
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}
		static void WriteText(std::string&& data, std::string filePath) {
			WriteText(data, filePath);
		}

		// Appends text to a text file
		static void AppendText(std::string& data, std::string filePath, int position = -1) {
			std::fstream fileStream(filePath, std::ios::in | std::ios::out);
			if (!fileStream.is_open()) {
				printf("Couldn't load text file \"%s\".", filePath.c_str());
				console::WaitFor(' ', true);
				exit(69);
			}

			if (position < 0) {
				fileStream.seekp(0, std::ios_base::end);
			}
			else {
				fileStream.seekp(position);
			}

			fileStream << data;
			fileStream.close();
		}
		static void AppendText(std::string&& data, std::string filePath, int position = -1) {
			AppendText(data, filePath, position);
		}

		// Returns a filedata from a given file
		static bytes ReadBytes(std::string filePath) {
			std::ifstream fileStream(filePath, std::ios::binary);
			if (!fileStream.is_open()) {
				printf("Couldn't load file \"%s\".", filePath.c_str());
				console::WaitFor(' ', true);
				exit(69);
			}
			fileStream.seekg(0, std::ios::end);
			size_t fileLen = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);
			bytes byteArray(fileLen);
			fileStream.read((char*)&byteArray[0], fileLen);
			fileStream.close();
			return byteArray;
		}

		// Writes bytes to a file
		static void WriteBytes(bytes& data, std::string filePath) {
			std::ofstream fileStream(filePath, std::ios::binary);
			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		static void WriteBytes(bytes&& data, std::string filePath) {
			WriteBytes(data, filePath);
		}

		// Appends bytes to a file
		static void AppendBytes(bytes& data, std::string filePath, int position = -1) {
			std::fstream fileStream(filePath, std::ios::in | std::ios::out | std::ios::binary);
			if (!fileStream.is_open()) {
				printf("Couldn't load file \"%s\".", filePath.c_str());
				console::WaitFor(' ', true);
				exit(69);
			}

			if (position < 0) {
				fileStream.seekp(0, std::ios_base::end);
			}
			else {
				fileStream.seekp(position);
			}

			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		static void AppendBytes(bytes&& data, std::string filePath, int position = -1) {
			AppendBytes(data, filePath, position);
		}

		// Deletes a given file
		static void Delete(std::string filePath) {
			remove(filePath.c_str());
		}
	};
}
