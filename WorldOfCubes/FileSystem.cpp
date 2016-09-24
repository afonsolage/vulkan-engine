#include "stdafx.h"
#include "FileSystem.h"

FileSystem::FileSystem()
{

}


FileSystem::~FileSystem()
{
	printf("File system destroyed.\n");
}

std::vector<std::string> FileSystem::list_files_on_folder(const std::string& path, const std::string& extension_filter) const
{
	namespace file_system = boost::filesystem;

	file_system::path root = { path.c_str() };

	if (!file_system::exists(root) || !file_system::is_directory(root))
		throw std::runtime_error("Invalid folder name!");

	file_system::recursive_directory_iterator it(root);
	file_system::recursive_directory_iterator end;

	std::vector<std::string> result;

	for (;it != end; it++)
	{
		if (file_system::is_regular_file(*it) && it->path().extension() == extension_filter)
		{
			result.emplace_back(it->path().generic_string());
		}
	}

	return result;
}

std::vector<char> FileSystem::read_file(const std::string& path_file) const
{
	std::ifstream file(path_file, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Failed to open file!");

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

std::string FileSystem::get_file_name(const std::string& path) const
{
	return boost::filesystem::path(path).filename().stem().generic_string();
}

ptree FileSystem::load_info_file(const std::string & path_file) const
{
	ptree ptree;

	boost::property_tree::info_parser::read_info(path_file, ptree);

	return ptree;
}
