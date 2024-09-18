#pragma once

#include <string>
#include <vector>

namespace Folder
{
	int create_folder_if_does_not_exist_already_no_message(std::string folderPath);

	int create_folder_if_does_not_exist_already(std::string folderPath);
	
	std::vector<std::string> getFilePathsInFolder(const std::string& folderPath);

	bool fileExists(const std::string& path);
}