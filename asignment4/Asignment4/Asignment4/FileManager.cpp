#include "FileManager.h"
#include <fstream>
#include <sstream>

bool FileManager::saveTextToFile(const std::string& path, const std::string& text) {
	std::ofstream file(path);

	if (!file.is_open()) {
		return false;
	}
	file << text;
	file.close();
	return true;
}

bool FileManager::loadTextFromFile(const std::string& path, std::string& text) {
	std::ifstream file(path);

	if (!file.is_open()) {
		return false;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	text = buffer.str();
	file.close();
	return true;
}