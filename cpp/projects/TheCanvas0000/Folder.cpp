#include <filesystem>
#include <iostream>

namespace Folder
{
	int create_folder_if_does_not_exist_already_no_message(std::string folderPath)
	{
		namespace fs = std::filesystem;
		if (!fs::exists(folderPath)) {
			if (fs::create_directory(folderPath)) {
				// std::cout << "Folder created successfully." << std::endl;
			}
			else {
				// std::cerr << "Failed to create folder." << std::endl;
				return 1;
			}
		}
		else {
			// std::cout << "Folder already exists." << std::endl;
		}
	}

	int create_folder_if_does_not_exist_already(std::string folderPath)
	{
		namespace fs = std::filesystem;
		if (!fs::exists(folderPath)) {
			if (fs::create_directory(folderPath)) {
				std::cout << "Folder created successfully." << std::endl;
			}
			else {
				std::cerr << "Failed to create folder." << std::endl;
				return 1;
			}
		}
		else {
			std::cout << "Folder already exists." << std::endl;
		}
	}

	std::vector<std::string> getFilePathsInFolder(const std::string& folderPath)
	{
		namespace fs = std::filesystem;
		std::vector<std::string> filePaths;

		try {
			for (const auto& entry : fs::directory_iterator(folderPath)) {
				if (entry.is_regular_file()) {
					filePaths.push_back(entry.path().string());
				}
			}
		}
		catch (const fs::filesystem_error& e) {
			std::cerr << "Filesystem error: " << e.what() << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "General exception: " << e.what() << std::endl;
		}

		return filePaths;
	}


	bool fileExists(const std::string& path)
	{
		return std::filesystem::exists(path);
	}
}