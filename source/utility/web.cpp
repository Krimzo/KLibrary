#include "utility/web.h"
#include "utility/console.h"
#include "utility/strings.h"
#include "math/math.h"

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")


std::string kl::web::download(const std::string& url, uint bufferSize) {
	HINTERNET connection = InternetOpenA("Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
	if (kl::console::warning(!connection, "Failed to open a browser")) {
		return {};
	}

	HINTERNET address = InternetOpenUrlA(connection, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
	if (kl::console::warning(!address, kl::format("Failed to open url \"", url, "\""))) {
		return {};
	}

	std::string data;
	std::vector<byte> buffer(bufferSize);
	for (DWORD bytesRead = 0; InternetReadFile(address, buffer.data(), DWORD(buffer.size()), &bytesRead) && bytesRead > 0;) {
		for (DWORD i = 0; i < bytesRead; i++) {
			data.push_back(buffer[i]);
		}
	}

	InternetCloseHandle(address);
	InternetCloseHandle(connection);

	return data;
}
