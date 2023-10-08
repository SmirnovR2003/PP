
#include <windows.h>
#include <string>
#include <iostream>
struct Point
{
	int x;
	int y;
};
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	Point point = *(Point*)(lpParam);
	ExitThread(0); // функция устанавливает код завершения потока в 0
}
int main(int argc, char* argv[])
{
	Point point{0,0};
	Point* pointPtr = &point;
	int threadCount = atoi(argv[1]);
	HANDLE* handles = new HANDLE[threadCount];
	for (int i = 0; i < threadCount; i++)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, i, CREATE_SUSPENDED, NULL);
	}
	// запуск двух потоков
	for (int i = 0; i < threadCount; i++)
	{
		ResumeThread(handles[i]);
	}
	// ожидание окончания работы потоков
	WaitForMultipleObjects(threadCount, handles, true, INFINITE);

	return 0;
}