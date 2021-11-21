#pragma once
#include <string>

namespace nEngine
{
	bool ReadFileToString(const std::string& filename, std::string& filestring);
	void SetFilePath(const std::string& pathname);
	std::string GetFilePath();
}