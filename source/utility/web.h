#pragma once

#include "libspec/using.h"
#include "utility/file.h"

#pragma comment(lib, "wininet.lib")


namespace kl {
	namespace web {
		// Downloads website data(bytes) from the given url
		String getWebsiteData(const String& url, int bufferSize = 65536);

		// Downloads data from the website and saves it in a file
		void downloadToFile(const String& url, const String& fileName);
	};
}
