#define EXAMPLE2
#ifdef EXAMPLE2
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "asset_meta.hpp"
#include "asset_manager.hpp"

void error_callback(const std::string& message)
{
	std::cout << "Asset manager reports error: " << message << ".\n";
}

std::optional<std::unique_ptr<std::any>> textfile_loader(asset_meta meta)
{
	std::ifstream fin(meta.filepath);

	if(!fin)
	{
		return {};
	}

	std::string result(
		(std::istreambuf_iterator<char>(fin)), 
		std::istreambuf_iterator<char>());

	return std::make_unique<std::any>(result);
}

int main()
{
	asset_manager am;

	am.set_loader<std::string>(textfile_loader);
	std::string result = *am.get_asset<std::string>("examples/test.txt");

	std::cout << result << std::endl;
}

#endif