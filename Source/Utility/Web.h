#pragma once

#include "Utility/File.h"


namespace kl::Web {
	String Download(const String& url, uint bufferSize = 65536);
}
