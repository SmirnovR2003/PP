// 1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;

struct PI
{
    double pi = 0;
    size_t n = 0;
    void Calculate()
    {
        pi += pow(-1, n) / (2 * n + 1);
        n++;
    }
};



void For(int n)
{
    PI myPI;
#pragma omp for
    for (int i = 0; i < n; i++) {
        myPI.Calculate();
    }
    cout << 4 * myPI.pi << endl;
}


void ParallelFor(int n)
{
    PI myPI;
#pragma omp parallel for 
    for (int i = 0; i < n; i++) {
        myPI.Calculate();
    }
    cout << 4 * myPI.pi << endl;
}


void ParallelForAndAtomic(int n)
{
    double pi = 0;
#pragma omp parallel for 
    for (int i = 0; i < n; i++) {
#pragma omp atomic
        pi += pow(-1, i) / (2 * i + 1);
    }
    cout << 4*pi << endl;
}


void Reduction(int n)
{
    double pi = 0;
#pragma omp parallel for reduction(+:pi)
    for (int i = 0; i < n; i++) {
        pi += pow(-1, i) / (2 * i + 1);
    }
    cout << 4*pi << endl;
}

void FuncWithPrivate()
{
    int x = 44;
#pragma omp parallel for lastprivate(x)
    for (int i = 0; i <= 10; i++) {
        x = i;
        printf("Thread number: %d x: %d\n", omp_get_thread_num(), x);
    }
    printf("x is %d\n", x);

}

int main()
{
    auto start = clock();
    For(50'000'000);
    cout << clock() - start << endl;
    start = clock();
    ParallelFor(50'000'000);
    cout << clock() - start << endl;
    start = clock();
    ParallelForAndAtomic(50'000'000);
    cout << clock() - start << endl;
    start = clock();
    Reduction(50'000'000);
    cout << clock() - start << endl;
    start = clock();

    //FuncWithPrivate();
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
