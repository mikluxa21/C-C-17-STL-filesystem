#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "worker.h"


int main() {
	setlocale(LC_ALL, "Russian");
	Worker worker;
	worker.Run();
	return 0;
}











//ShellExecuteA(NULL, "open", "notepad.exe", "C:\\Users\\79523\\YandexDisk\\4.2\\Savelev\\labs\\3\\code\\Project2\\test.txt", NULL, SW_SHOWNORMAL);
