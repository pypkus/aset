#include <iostream>
#include <vector>
#include "asset_manager.hpp"

std::unique_ptr<std::any> my_custom_loader(asset_meta meta)
{
	return std::make_unique<std::any>(std::string("Test std::string!") + meta.filename);
}

std::unique_ptr<std::any> my_imaginary_loader(asset_meta meta)
{
	return std::make_unique<std::any>(std::string("Loaded the thing! ") + meta.filepath);
}

void my_error_callback(const std::string& message)
{
	std::cout << "Asset manager reports error: " << message << ".\n";
}

int main() {
	///////////////////////
	// Manager test
	//
	asset_manager am;
	am.set_error_callback_fn(my_error_callback);

	// Assign custom loader
	am.set_loader<std::string>(my_custom_loader);

	// Preload asset
	am.load_asset<std::string>("aaa");

	// Get the asset (should be preloaded)
	auto data_aaa = am.get_asset<std::string>("aaa");
	std::cout << *data_aaa << std::endl;

	// Forget the asset and load it again
	am.forget_asset("aaa");
	data_aaa = am.get_asset<std::string>("aaa");

	// Load asset without ever preloading it
	auto data_bbb = am.get_asset<std::string>("bbb");

	// Should invoke error callback
	auto data_ccc = am.get_asset<float>("bad_type");

	std::cout << *data_aaa << std::endl;
	std::cout << *data_bbb << std::endl;

	///////////////////////
	// Load asset without specifying type
	//
	std::vector<std::string> str_a{ ".txt", ".cfg" };

	am.set_loader<std::string>(my_imaginary_loader, str_a.begin(), str_a.end());
	am.load_asset("my_imaginary_file.txt");
	am.load_asset("my_imaginary_file.cfg");
	am.load_asset("my_imaginary_file.bad");
	auto ima_thing1 = am.get_asset<std::string>("my_imaginary_file.txt");
	auto ima_thing2 = am.get_asset<std::string>("my_imaginary_file.cfg");

	std::cout << *ima_thing1 << std::endl << *ima_thing2 << std::endl;

	// Perform test where asset doesn't exist and is requested to unload
	am.forget_asset("some_nonexistsing_asset.txt");
}