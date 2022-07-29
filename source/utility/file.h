#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "render/vertex.h"


namespace kl
{
	class file
	{
		FILE* m_File = nullptr;

	public:
		static std::string extension(const std::string& filePath);
		static std::vector<std::string> getFiles(const std::string& dirPath, bool recursive = false);
		static std::string readString(const std::string& filePath);
		static bool writeString(const std::string& filePath, const std::string& data);
		static bool appendString(const std::string& filePath, const std::string& data, int position = -1);
		static std::vector<kl::vertex> parseMesh(const std::string& filePath, bool flipZ = true);

		file();
		file(const std::string& filePath, bool clear = true);
		file(const kl::file&) = delete;
		void operator=(const kl::file&) = delete;
		~file();

		bool open(const std::string& filePath, bool clear = true);
		bool close();
		bool isOpen() const;

		bool seek(int64 position);
		bool move(int64 delta);
		bool rewind();
		bool unwind();
		int64 tell() const;

		template<typename T>
		bool read(T& object) const
		{
			if (m_File)
			{
				return bool(fread(&object, sizeof(T), 1, m_File));
			}
			return false;
		}

		template<typename T>
		uint64 read(T* buffer, uint64 count) const
		{
			if (m_File)
			{
				return fread(buffer, sizeof(T), count, m_File);
			}
			return false;
		}

		template<typename T>
		bool write(const T& object)
		{
			if (m_File)
			{
				return bool(fwrite(&object, sizeof(T), 1, m_File));
			}
			return false;
		}

		template<typename T>
		uint64 write(const T* buffer, uint64 count)
		{
			if (m_File)
			{
				return fwrite(buffer, sizeof(T), count, m_File);
			}
			return false;
		}
	};
}
