#pragma once

#include "utility/file.h"


namespace kl::Web {
	String Download(const String& url, uint bufferSize = 65536);
}
