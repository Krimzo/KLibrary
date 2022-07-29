#pragma once

#include "utility/file.h"


namespace kl
{
	namespace web
	{
		std::string siteData(const std::string& url, uint bufferSize = 65536);
		void toFile(const std::string& url, const std::string& fileName);
	};
}
