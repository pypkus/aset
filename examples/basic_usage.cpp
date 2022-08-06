#include <iostream>
#include "asset_manager.hpp"

std::unique_ptr<std::any> my_custom_loader(asset_meta meta)
{
	return std::make_unique<std::any>(std::string("Test std::string!") + meta.filename);
}

int main() {
	//
	// manager test
	asset_manager am;

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

	std::cout << *data_aaa << std::endl;
	std::cout << *data_bbb << std::endl;
}