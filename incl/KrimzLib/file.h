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
				printf("Couldn't load text file \"%s\".\n", filePath.c_str());
				kl::console::WaitFor(' ', true);
				exit(69);
			}
			textBuffer << fileStream.rdbuf();
			fileStream.close();
			return textBuffer.str();
		}

		// Writes text to a text file
		static void WriteText(std::string filePath, std::string& data) {
			std::ofstream fileStream(filePath);
			fileStream << data;
			fileStream.close();
		}
		static void WriteText(std::string filePath, std::string&& data) {
			WriteText(filePath, data);
		}

		// Appends text to a text file
		static void AppendText(std::string filePath, std::string& data, int position = -1) {
			std::fstream fileStream(filePath, std::ios::in | std::ios::out);
			if (!fileStream.is_open()) {
				printf("Couldn't load text file \"%s\".\n", filePath.c_str());
				kl::console::WaitFor(' ', true);
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
		static void AppendText(std::string filePath, std::string&& data, int position = -1) {
			AppendText(filePath, data, position);
		}

		// Returns a filedata from a given file
		static bytes ReadBytes(std::string filePath) {
			std::ifstream fileStream(filePath, std::ios::binary);
			if (!fileStream.is_open()) {
				printf("Couldn't load file \"%s\".\n", filePath.c_str());
				kl::console::WaitFor(' ', true);
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
		static void WriteBytes(std::string filePath, bytes& data) {
			std::ofstream fileStream(filePath, std::ios::binary);
			fileStream.write((char*)&data[0], data.size());
			fileStream.close();
		}
		static void WriteBytes(std::string filePath, bytes&& data) {
			WriteBytes(filePath, data);
		}

		// Appends bytes to a file
		static void AppendBytes(std::string filePath, bytes& data, int position = -1) {
			std::fstream fileStream(filePath, std::ios::in | std::ios::out | std::ios::binary);
			if (!fileStream.is_open()) {
				printf("Couldn't load file \"%s\".\n", filePath.c_str());
				kl::console::WaitFor(' ', true);
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
		static void AppendBytes(std::string filePath, bytes&& data, int position = -1) {
			AppendBytes(filePath, data, position);
		}

		// Deletes a given file
		static void Delete(std::string filePath) {
			remove(filePath.c_str());
		}
	};
}