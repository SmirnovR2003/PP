#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <time.h>
#include <winapifamily.h>
//#include <chrono>
using namespace std;
using namespace cv;

struct PairPoints
{
	Point leftTop, rightBottom;
};

typedef vector<PairPoints> Rectangles;

Mat img;
Mat Blur_img1;

int threadCount;
int rectWidth;
int rectHeight;
PairPoints GetNextSquare(PairPoints curr)
{
	if (curr.rightBottom.y + rectHeight < img.rows)
	{
		curr.rightBottom.y += rectHeight;
	}
	else
	{
		curr.rightBottom.y = rectHeight;
	}
	if (curr.rightBottom.x + rectWidth < img.cols)
	{
		curr.rightBottom.x += rectWidth;
	}
	else
	{
		curr.rightBottom.x = rectWidth;
	}
	curr.leftTop.x = curr.rightBottom.x - rectWidth;
	curr.leftTop.y = curr.rightBottom.y - rectHeight;
	return curr;
}

Rectangles squares;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	int count = *(int*)lpParam;
	PairPoints rect = squares[count];
	

	for (int y = rect.leftTop.y+2;y < rect.rightBottom.y-2;y++)
		for (int x = rect.leftTop.x+2;x < rect.rightBottom.x-2;x++)
		{
			float totalRed = 0;
			float totalGreen = 0;
			float totalBlue = 0;
			uchar blue = 0;
			uchar green = 0;
			uchar red = 0;
			for (int fy = -2; fy <= 2; fy++)
			{
				for (int fx = -2; fx <= 2; fx++)
				{
					Vec3b pixel = img.at<Vec3b>(y+fy, x+fx);
					totalRed += pixel[0];
					totalGreen += pixel[1];
					totalBlue += pixel[2];
				}
			}
			Vec3b pixel = img.at<Vec3b>(y , x );
			pixel[0] = totalRed / 25;
			pixel[1] = totalGreen / 25;
			pixel[2] = totalBlue / 25;
			Blur_img1.at<Vec3b>(y, x) = pixel;
		}

	for (int i = 1;i < threadCount; i++)
	{
		rect = GetNextSquare(rect);
		for (int y = rect.leftTop.y+2;y < rect.rightBottom.y-2;y++)
			for (int x = rect.leftTop.x+2;x < rect.rightBottom.x-2;x++)
			{
				float totalRed = 0;
				float totalGreen = 0;
				float totalBlue = 0;
				uchar blue = 0;
				uchar green = 0;
				uchar red = 0;
				for (int fy = -2; fy <= 2; fy++)
				{
					for (int fx = -2; fx <= 2; fx++)
					{
						Vec3b pixel = img.at<Vec3b>(y + fy, x + fx);
						totalRed += pixel[0];
						totalGreen += pixel[1];
						totalBlue += pixel[2];
					}
				}
				Vec3b pixel = img.at<Vec3b>(y, x);
				pixel[0] = totalRed / 25;
				pixel[1] = totalGreen / 25;
				pixel[2] = totalBlue / 25;
				Blur_img1.at<Vec3b>(y, x) = pixel;
			}
	}


	ExitThread(0); // функция устанавливает код завершения потока в 0
}

void  setNumCPUs(int num)
{
	DWORD_PTR processAffinityMask;
	DWORD_PTR systemAffinityMask;
	if (GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask)) {
		// Count up the number of processes available for our app.
		int n = 0;
		for (int i = 0; i < sizeof(DWORD_PTR) * 4; i++) {
			if ((systemAffinityMask & ((DWORD_PTR)1 << i))) n++;
		}

		// Limit max CPUs by user configuration.
		DWORD_PTR newAffinityMask = 0;
		if (num < n) {
			int m = 0;
			for (int i = 0; i < sizeof(DWORD_PTR) * 4; i++) {
				if ((systemAffinityMask & ((DWORD_PTR)1 << i))) {
					newAffinityMask |= ((DWORD_PTR)1 << i);
					m++;
					if (m == num) break;
				}
			}
			newAffinityMask <<= (n - num);
		}
		else {
			newAffinityMask = systemAffinityMask;
		}

		// Set new affinity mask.
		SetProcessAffinityMask(GetCurrentProcess(), newAffinityMask);
		
		
	}
}


int main(int argc, char* argv[])
{
	string inPath("test5.bmp");
	string outPath("out.bmp");
	threadCount = 1;//atoi(argv[3]);
	int kernelCount = 1;//atoi(argv[4]);
	img = imread(inPath);
	Blur_img1 = img.clone();

	Scalar sc;
	sc.val;

	cout << 123 << endl;
	rectWidth = (img.cols)/ threadCount;
	rectHeight = (img.rows)/ threadCount;


	setNumCPUs(kernelCount);

	for (int i = 0; i < threadCount; ++i)
	{
		squares.push_back({ { i * rectHeight, 0 }, {(i+1) * rectHeight,rectWidth} });
	}

	HANDLE* handles = new HANDLE[threadCount];
	vector<int> vec(threadCount);
	for (int k = 0; k < 10; k++)
	{
		Blur_img1 = img.clone();
		for (int i = 0; i < threadCount; i++)
		{
			vec[i] = i;
			handles[i] = CreateThread(NULL, 0, &ThreadProc, &vec[i], CREATE_SUSPENDED, NULL);
			SetThreadAffinityMask(handles[i], static_cast<DWORD_PTR>(1) << (i / kernelCount + 1));
		}
		// запуск двух потоков
		for (int i = 0; i < threadCount; i++)
		{
			ResumeThread(handles[i]);
		}
		// ожидание окончания работы потоков
		WaitForMultipleObjects(threadCount, handles, true, INFINITE);
		

	}

	cout << (int)clock();
	imwrite("test-out.bmp", Blur_img1);
	
	system("pause");

	return 0;
}














































