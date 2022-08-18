#pragma once

#include <iostream>
#include <string>
#include <mutex>

#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <propvarutil.h>
#include <windows.h>

#include "Graphics/Image.h"


namespace kl {
	class Video {
		IMFSourceReader* m_Reader = nullptr;

	public:
		Video();
		Video(const String& filePath);
		Video(const Video&) = delete;
		void operator=(const Video&) = delete;
		~Video();

		operator bool() const;

		bool isOpen() const;

		void open(const String& filePath);
		void close();

		uint64 size() const;

		float duration() const;

		float fps() const;

		uint frameCount() const;

		UInt2 frameSize() const;

		bool getFrame(Image& out);
	};
}
