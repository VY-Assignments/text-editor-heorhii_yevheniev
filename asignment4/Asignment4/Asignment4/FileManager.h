#pragma once
#include <string>

class FileManager {
public:
	static bool saveTextToFile(const std::string& path, const std::string& text);
	static bool loadTextFromFile(const std::string& path, std::string& text);
};