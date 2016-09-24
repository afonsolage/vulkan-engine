#pragma once

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

using ptree = boost::property_tree::ptree;

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	std::vector<std::string> list_files_on_folder(const std::string& path_folder, const std::string& extension_filter) const;
	std::vector<char> read_file(const std::string& path_file) const;
	std::string get_file_name(const std::string& path) const;

	ptree load_info_file(const std::string& path_file) const;
};

