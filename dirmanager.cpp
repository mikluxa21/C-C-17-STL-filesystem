#include "dirmanager.h"

void ToNormal::Replace(std::string* path, char sim_whith, std::string str_to)
{
	std::string path_to_out;
	for (char c : *path)
	{
		if (c == sim_whith)
		{
			path_to_out+=str_to;
		}
		else
		{
			path_to_out.push_back(c);
		}
	}
	*path = path_to_out;
}


std::string PathManager::FindFileName(std::string path)
{
	std::string file_name;
	for (size_t i = path.find_last_of('\\') + 1; i < path.find_last_of('.'); i++)
	{
		file_name.push_back(path[i]);
	}
	return file_name;
}

std::string PathManager::FindPath(std::string path_with_fname)
{
	std::string path;
	for (size_t i = 0; i < path_with_fname.find_last_of('\\') + 1; i++)
	{
		path.push_back(path_with_fname[i]);
	}
	return path;
}

std::string PathManager::FindPrevPath(std::string path_with_fname)
{

	int i = path_with_fname.length() - 1;
	for (int j = 0; j < 3; j++)
	{
		while (path_with_fname[i] != '\\')
		{
			path_with_fname.pop_back();
			i--;
		}
		path_with_fname.pop_back();
		i--;
	}
	return path_with_fname;
}


std::vector<std::string> DirWork::GetAllTxtPath()
{
	std::vector<std::string> path_vector;
	fs::path dir_path = fs::current_path();
	for (const auto& file : fs::directory_iterator(dir_path))
	{
		fs::path file_fs = file;
		std::string file_str = file_fs.generic_string();

		if (file_str[file_str.length() - 1] == 't' and
			file_str[file_str.length() - 2] == 'x' and
			file_str[file_str.length() - 3] == 't')
		{
			path_vector.push_back(file_str);
		}
	}
	return path_vector;
}

std::vector<std::string> DirWork::GetAllExePath(std::string sdir_path)
{
	std::vector<std::string> path_vector;
	fs::path dir_path(sdir_path);
	for (const auto& file : fs::directory_iterator(dir_path))
	{
		fs::path file_fs = file;
		std::string file_str = file_fs.generic_string();

		if (file_str[file_str.length() - 1] == 'e' and
			file_str[file_str.length() - 2] == 'x' and
			file_str[file_str.length() - 3] == 'e')
		{
			path_vector.push_back(file_str);
		}
	}
	return path_vector;
}

std::string DirWork::GetCurrentExePath()
{
	const int NAMELEN = 256;
	TCHAR name[NAMELEN];

	GetModuleFileName(NULL, name, NAMELEN); // Получаем имя файла

	std::wstring wString(&name[0]);
	std::string convStr(wString.begin(), wString.end());

	return convStr;
}


std::vector<std::string> DirManager::GetAllTxtPath()
{
	std::vector<std::string> all_txt_path;
	for (std::string str : dir_work.GetAllTxtPath())
	{
		path_manager.Replace(&str, '/', "\\");
		all_txt_path.push_back(str);
	}
	return all_txt_path;
}

std::vector<std::string> DirManager::GetAllExePath(std::string sdir_path)
{
	std::vector<std::string> all_txt_path;
	for (std::string str : dir_work.GetAllExePath(sdir_path))
	{
		path_manager.Replace(&str, '/', "\\");
		all_txt_path.push_back(str);
	}
	return all_txt_path;
}

std::string DirManager::GetCurrentExePath()
{
	std::string current_exe_path = dir_work.GetCurrentExePath();
	path_manager.Replace(&current_exe_path, '\\', "\\\\");
	return current_exe_path;
}