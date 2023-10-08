
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include<ctime>

using namespace std;

clock_t start;

ofstream out("out.txt");
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	int number = *(int*)(lpParam);
	for (int i = 0; i < 20; i++)
	{
		for (int i = 0; i < 1000000; i++)
		{
			int a = 1;
		}
		string str = " " + to_string(number) + " " + to_string(clock()-start) + "\n";
		out << str;
	}
	ExitThread(0); // функция устанавливает код завершения потока в 0
}
int main(int argc, char* argv[])
{
	system("pause");
	start = clock();
	int one = 1;
	int two = 2;
	HANDLE* handles = new HANDLE[2];
	handles[0] = CreateThread(NULL, 0, &ThreadProc, &one, CREATE_SUSPENDED, NULL);
	handles[1] = CreateThread(NULL, 0, &ThreadProc, &two, CREATE_SUSPENDED, NULL);
	SetThreadPriority(handles[0], 0);
	SetThreadPriority(handles[1], 1);
	// запуск двух потоков
	for (int i = 0; i < 2; i++)
	{
		ResumeThread(handles[i]);
	}
	// ожидание окончания работы потоков
	WaitForMultipleObjects(2, handles, true, INFINITE);

	return 0;
}