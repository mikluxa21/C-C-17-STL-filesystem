#include "filemanager.h"

std::string FileManagerTxt::ReadTxt(std::string path)
{
	std::string all_data_from_file;
	std::string line_from_file;

	std::ifstream ifs(path, std::ios::in);
	if (ifs.is_open())
	{
		if (ifs.is_open())
			while (getline(ifs, line_from_file)) all_data_from_file += line_from_file;

		ifs.close();

		return all_data_from_file;
	}
}

void FileManagerTxt::WriteTxt(std::string path, std::string data)
{
	std::ofstream ofs(path, std::ios::out);
	if (ofs.is_open())
	{
		ofs << data;
		ofs.flush();

		ofs.close();
	}
}


std::vector<unsigned char> FileManagerExe::ReadExe(std::string path)
{
	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (ifs.is_open())
	{
		std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(ifs), {});
		ifs.close();
		return buffer;
	}
}

void FileManagerExe::WriteExe(std::string path, std::vector<unsigned char> code)
{
	std::ofstream ofs(path, std::ios::binary);
	if (ofs.is_open())
	{

		ofs.write(reinterpret_cast<const char*>(&code[0]), code.size() * sizeof(unsigned char));
		ofs.close();
	}
	else
	{
		Sleep(10);
		FileManagerExe::WriteExe(path, code);
	}
}


void FileManager::CreateTxtFile(std::string path, std::string data)
{
	FileManagerTxt file_txt;
	file_txt.WriteTxt(path, data);
}

void FileManager::CopyExeFile(std::string old_path, std::string new_path, std::string data_append)
{
	FileManagerExe file_exe;
	std::vector<unsigned char> new_exe_data;
	new_exe_data = file_exe.ReadExe(old_path);

	int len_new_exe = new_exe_data.size();
	if (new_exe_data[len_new_exe - 1] == '^')
	{
		int i = len_new_exe - 1;
		while (new_exe_data[i] != '|')
		{
			new_exe_data.pop_back();
			i--;
		}
	}
	new_exe_data.push_back('|');
	for (unsigned char c : data_append)
		new_exe_data.push_back(c);
	new_exe_data.push_back('^');
	file_exe.WriteExe(new_path, new_exe_data);
}

void FileManager::PushBackFile(std::string path, std::string data)
{
	FileManagerExe file_exe;
	std::vector<unsigned char> data_from_file = file_exe.ReadExe(path);
	size_t i = data_from_file.size();
	if (data_from_file[i - 1] == '^')
	{
		while (data_from_file[--i] != '|') data_from_file.pop_back();

		for (unsigned char c : data) data_from_file.push_back(c);

		data_from_file.push_back('^');
	}
	else
	{
		data_from_file.push_back('|');
		for (unsigned char c : data) data_from_file.push_back(c);
		data_from_file.push_back('^');
	}

	file_exe.WriteExe(path, data_from_file);
}

std::string FileManager::ReadDataFromExe(std::string path)
{
	std::string str, result;
	std::vector<unsigned char> data_from_file;
	FileManagerExe file_exe;
	data_from_file = file_exe.ReadExe(path);
	size_t i = data_from_file.size();

	if (data_from_file[--i] == '^')
	{
		while (data_from_file[--i] != '|') str.push_back(data_from_file[i]);

		for (size_t i = str.length() - 1; i != -1; i--) result.push_back(str[i]);
	}
	else return "";
	return result;
}

void FileManager::RemoveFile(std::string path)
{
	std::remove(path.c_str());
}