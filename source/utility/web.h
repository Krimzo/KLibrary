#pragma once

#include "utility/file.h"


namespace kl {
	namespace web {
		std::string download(const std::string& url, uint bufferSize = 65536);
	};
}
