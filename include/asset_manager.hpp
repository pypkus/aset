#pragma once
#include <any>
#include <cstddef>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <optional>
#include <utility>

#include "asset_meta.hpp"

// TODO: Multithreading
// TODO: Allow loading assets only with meta (no type required)
// TODO: Implement callback system to measure performance of the system
//	- File loaded
//	- Data amount loaded
class asset_manager 
{
private:
	using TID = std::type_index;
	using loader_fn = std::function<std::unique_ptr<std::any>(asset_meta)>;
	using writer_fn = std::function<void(const asset_meta&, const std::any&)>;

public:
	using asset_t = std::unique_ptr<std::any>;
	using ref_asset_t = std::reference_wrapper<asset_t>;
	using opt_asset_t = std::optional<ref_asset_t>;
	using callback_fn = std::function<void(const std::string&)>;

public:

	asset_manager() = default;

	//
	// Custom file access
	//
	template <typename T>
	void set_loader(loader_fn fn);

	template <typename T>
	void set_writer(writer_fn fn);

	//
	// Asset retrieval
	//

	// Returns asset or returns nullptr if couldn't retrieve asset
	template <typename T>
	T* get_asset(const std::string& filepath);

	// Writes asset to file using custom writer function
	template <typename T>
	void write_asset(const std::string& filepath, const T& asset);

	//
	// Asset storing
	//
	template <typename T>
	opt_asset_t load_asset(const std::string& filepath);

	// TODO: Preloads asset using asset_meta. Does not require to specify a type
	// void load_asset(const asset_meta& asset);

	// Unloads asset. Does not require to specify a type
	void forget_asset(const std::string& filepath);

	//
	// Error handling
	//
	void set_error_callback_fn(callback_fn fn);
	void set_infos_callback_fn(callback_fn fn);

	void invoke_error_cb(const std::string& message);
	void invoke_infos_cb(const std::string& message);

private:
	std::map<TID, loader_fn> m_loaders;
	std::map<TID, writer_fn> m_writers;

	std::unordered_map<std::string, asset_t> m_assets;

	callback_fn m_err_cb, m_infos_cb;
};

inline void asset_manager::set_error_callback_fn(asset_manager::callback_fn fn)
{
	m_err_cb = fn;
}

inline void asset_manager::set_infos_callback_fn(asset_manager::callback_fn fn)
{
	m_infos_cb = fn;
}

inline void asset_manager::invoke_error_cb(const std::string& message)
{
	if(m_err_cb)
	{
		m_err_cb(message);
	}
}

inline void asset_manager::invoke_infos_cb(const std::string& message)
{
	if(m_infos_cb)
	{
		m_infos_cb(message);
	}
}

template<typename T>
void asset_manager::set_loader(asset_manager::loader_fn fn)
{
	TID id = std::type_index(typeid(T));
	m_loaders[id] = fn;
}

template<typename T>
void asset_manager::set_writer(asset_manager::writer_fn fn)
{
	TID id = std::type_index(typeid(T));
	m_writers[id] = fn;
}

template<typename T>
asset_manager::opt_asset_t asset_manager::load_asset(const std::string& filepath)
{
	// Check if loader exists
	TID id = std::type_index(typeid(T));
	if(m_loaders.find(id) == m_loaders.end())
	{
		invoke_error_cb("loader not found");
		return std::nullopt;
	}

	// Load assset
	asset_meta metapath = asset_meta::construct_meta_from_filepath(filepath);
	auto result = m_loaders[id](metapath);

	if(result == nullptr)
	{
		invoke_error_cb(
			"loader of type \"" + std::string(typeid(T).name()) + "\" failed to load file \"" + metapath.filepath + "\"");
		return std::nullopt;
	}

	// Asset is being reloaded; replaced.
	if(m_assets.find(filepath) != m_assets.end())
	{
		m_assets.erase(filepath);
	}

	// Store
	return m_assets[filepath] = std::move(result);
}

template<typename T>
T* asset_manager::get_asset(const std::string& filepath)
{
	// check if asset is not loaded
	if (m_assets.find(filepath) == m_assets.end())
	{
		// load asset
		// Invokes error callback if loader not found on itself
		asset_manager::opt_asset_t result = load_asset<T>(filepath);
		if(result == std::nullopt)
		{
			// Return nullptr if failed to load
			return nullptr;
		}
	}

	return std::any_cast<T>(m_assets[filepath].get());
}

template<typename T>
void asset_manager::write_asset(const std::string& filepath, const T& asset)
{
	TID id = std::type_index(typeid(T));
	if(m_writers.find(id) == m_writers.end())
	{
		invoke_error_cb("no writer found");
		return;
	}

	m_writers[id](asset_meta::construct_meta_from_filepath(filepath), std::make_any(&asset));
}

inline void asset_manager::forget_asset(const std::string& filepath)
{
	m_assets.erase(filepath);
}