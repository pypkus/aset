#pragma once
#include <string>
#include <filesystem>

class asset_meta 
{
public:
	std::string filepath; 	// name of file with the path
	std::string filename;	// name of file without the path and without the extension
	std::string extension;	// file extension

	static asset_meta construct_meta_from_filepath(const std::string& filepath)
	{
		return
		{
			filepath,
			std::filesystem::path(filepath).filename().replace_extension("").string(),
			std::filesystem::path(filepath).extension().string()
		};
	}
};