#include "filemanager.h"
#include "dirmanager.h"
#include <thread>
#include <cstring>
#include <windows.h>
#include <iostream>

#pragma once
class Notepad
{
	bool state = 1;
	PROCESS_INFORMATION pi;
public:
	void OpenNotepad(std::string path);

	void WaitNotepade();

	bool GetState() { return state; }
};


class FileChanged
{
	bool status = 0;
	std::string strFileBaseDir;
	std::string strFileName;
public:
	void WatchFileChanges();

	bool GetState() { return status; };

	void SetParam(std::string new_strFileBaseDir, std::string new_strFileName) 
	{
		strFileBaseDir = new_strFileBaseDir;
		strFileName = new_strFileName;
	}
};


class FileWorker
{
public:
	void WaitTxt(std::string path_txt, std::string path_exe);

	void ReplaceTxt();
};


class ProgrammWorker
{
public:
	bool IsDublicate();

	bool DeleteDublicate();

	void RunNotepad();

	void RunTestExe();
};

class Worker
{
public:
	int Run();

};

