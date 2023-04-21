#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <cstdio>

#pragma once
class FileManagerTxt
{
public:
	std::string ReadTxt(std::string path);

	void WriteTxt(std::string path, std::string data);
};

class FileManagerExe
{
public:
	std::vector<unsigned char> ReadExe(std::string path);

	void WriteExe(std::string path, std::vector<unsigned char> code);
};

class FileManager : public FileManagerTxt
{

public:
	void CreateTxtFile(std::string path, std::string data);

	void CopyExeFile(std::string old_path, std::string new_path, std::string data_append);

	void PushBackFile(std::string path, std::string data);

	std::string ReadDataFromExe(std::string path);

	void RemoveFile(std::string path);

};

