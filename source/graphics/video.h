#pragma once

#include <iostream>
#include <string>
#include <mutex>

#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <propvarutil.h>
#include <windows.h>

#include "libspec/using.h"
#include "graphics/image.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "propsys.lib")


namespace kl {
	class video {
	private:
		IMFSourceReader* reader = nullptr;

	public:
		video();
		video(const String& filePath);
		video(const kl::video&) = delete;
		void operator=(const kl::video&) = delete;
		~video();

		void open(const String& filePath);
		void close();

		bool isOpen() const;
		float duration() const;
		size_t byteSize() const;
		kl::int2 frameSize() const;
		float frameRate() const;
		bool getFrame(kl::image& out);
	};
}
