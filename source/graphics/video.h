#pragma once

#include <iostream>
#include <string>
#include <mutex>

#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <propvarutil.h>
#include <windows.h>

#include "graphics/image.h"


namespace kl {
	class video {
		IMFSourceReader* m_Reader = nullptr;

	public:
		video();
		video(const std::string& filePath);
		video(const kl::video&) = delete;
		void operator=(const kl::video&) = delete;
		~video();

		operator bool() const;

		void open(const std::string& filePath);
		void close();

		uint64 size() const;
		float duration() const;

		float fps() const;
		uint count() const;
		kl::uint2 scale() const;
		bool get(kl::image& out);
	};
}
