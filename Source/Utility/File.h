#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Render/Vertex.h"


namespace kl {
	namespace Files {
		String GetExtension(const String& filePath);
		Vector<String> GetFiles(const String& dirPath, bool recursive = false);

		String ReadString(const String& filePath);
		bool WriteString(const String& filePath, const String& data);
		bool AppendString(const String& filePath, const String& data, int position = -1);

		Vector<Vertex> ParseMesh(const String& filePath, bool flipZ = true);
	}

	class File {
		FILE* m_File = nullptr;

	public:
		File();
		File(const String& filePath, bool clear = true);
		File(const File&) = delete;
		void operator=(const File&) = delete;
		~File();

		operator bool() const;

		bool isOpen() const;

		bool open(const String& filePath, bool clear = true);
		bool close();

		bool seek(int64 position);
		bool move(int64 delta);
		bool rewind();
		bool unwind();
		int64 tell() const;

		template<typename T>
		uint64 read(T& object) const {
			return read(&object, 1);
		}

		template<typename T>
		uint64 read(T* buffer, uint64 count) const {
			if (m_File) {
				return fread(buffer, sizeof(T), count, m_File);
			}
			return 0;
		}

		template<typename T>
		uint64 write(const T& object) {
			return write(&object, 1);
		}

		template<typename T>
		uint64 write(const T* buffer, uint64 count) {
			if (m_File) {
				return fwrite(buffer, sizeof(T), count, m_File);
			}
			return 0;
		}
	};
}
