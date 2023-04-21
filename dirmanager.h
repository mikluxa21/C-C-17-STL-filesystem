#include <filesystem>
#include <vector>
#include <string>
#include <Windows.h>

namespace fs = std::filesystem;

#pragma once
class ToNormal
{
public:
	void Replace(std::string* path, char sim_whith, std::string str_to);
};


class PathManager : public ToNormal
{
public:
	std::string FindFileName(std::string path);

	std::string FindPath(std::string path_with_fname);

	std::string FindPrevPath(std::string path_with_fname);
};


class DirWork final
{
public:
	std::vector<std::string> GetAllTxtPath();

	std::vector<std::string> GetAllExePath(std::string sdir_path);

	std::string GetCurrentExePath();
};


class DirManager : public PathManager
{
private:
	PathManager path_manager;
	DirWork dir_work;

public:
	std::vector<std::string> GetAllTxtPath();

	std::vector<std::string> GetAllExePath(std::string sdir_path);

	std::string GetCurrentExePath();

	void CreateDirection(std::string path) { fs::create_directories(path); }

};



