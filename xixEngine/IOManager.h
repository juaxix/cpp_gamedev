#pragma once
#include <vector>
#include <string>
#include <fstream>
namespace xixEngine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);

	};
}
