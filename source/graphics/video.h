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
	private:
		IMFSourceReader* m_Reader = nullptr;

	public:
		video();
		video(const std::string& filePath);
		video(const kl::video&) = delete;
		void operator=(const kl::video&) = delete;
		~video();

		void open(const std::string& filePath);
		void close();

		bool isOpen() const;
		float duration() const;
		uint64 byteSize() const;
		kl::uint2 frameSize() const;
		float frameRate() const;
		bool getFrame(kl::image& out);
	};
}
