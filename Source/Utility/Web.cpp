#include "Utility/Web.h"
#include "Utility/Console.h"
#include "Utility/Strings.h"
#include "Math/Math.h"

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")


kl::String kl::Web::Download(const String& url, uint bufferSize) {
	HINTERNET connection = InternetOpenA("Browser", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
	if (Warning(!connection, "Failed to open a browser")) {
		return {};
	}

	HINTERNET address = InternetOpenUrlA(connection, url.c_str(), nullptr, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
	if (Warning(!address, Format("Failed to open url \"", url, "\""))) {
		return {};
	}

	String data;
	Vector<byte> buffer(bufferSize);
	for (DWORD bytesRead = 0; InternetReadFile(address, buffer.data(), DWORD(buffer.size()), &bytesRead) && bytesRead > 0;) {
		for (DWORD i = 0; i < bytesRead; i++) {
			data.push_back(buffer[i]);
		}
	}

	InternetCloseHandle(address);
	InternetCloseHandle(connection);

	return data;
}
