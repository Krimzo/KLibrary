#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "render/vertex.h"


namespace kl {
	class file {
		FILE* m_File = nullptr;

	public:
		class get {
			get() {}
		public:
			static std::string extension(const std::string& filePath);
			static std::vector<std::string> files(const std::string& dirPath, bool recursive = false);
		};

		class strings {
			strings() {}
		public:
			static std::string read(const std::string& filePath);
			static bool write(const std::string& filePath, const std::string& data);
			static bool append(const std::string& filePath, const std::string& data, int position = -1);
		};

		class parse {
			parse() {}
		public:
			static kl::mesh mesh(const std::string& filePath, bool flipZ = true);
		};

		file();
		file(const std::string& filePath, bool clear = true);
		file(const kl::file&) = delete;
		void operator=(const kl::file&) = delete;
		~file();

		operator bool() const;

		bool open(const std::string& filePath, bool clear = true);
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
