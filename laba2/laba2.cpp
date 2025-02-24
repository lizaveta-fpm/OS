#include <iostream>
#include <windows.h>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// Глобальные переменные
vector<int> numbers;
double average_value;
int min_value;
int max_value;
HANDLE hMinMax, hAverage;

// Поток для нахождения минимума и максимума
DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    min_value = *min_element(numbers.begin(), numbers.end());
    max_value = *max_element(numbers.begin(), numbers.end());

    // Имитируем время работы с помощью Sleep
    Sleep(7);
    cout << "Min: " << min_value << ", Max: " << max_value << endl;

    return 0;
}

// Поток для вычисления среднего арифметического
DWORD WINAPI AverageThread(LPVOID lpParam) {
    average_value = accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size();

    // Имитируем время работы с помощью Sleep
    Sleep(12);
    cout << "Average: " << average_value << endl;

    return 0;
}

int main() {
    int size;

    // Ввод размерности и элементов массива
    cout << "Enter the size of the array: ";
    cin >> size;

    numbers.resize(size);
    cout << "Enter " << size << " integers: ";
    for (int i = 0; i < size; ++i) {
        cin >> numbers[i];
    }

    // Создание потоков
    hMinMax = CreateThread(NULL, 0, MinMaxThread, NULL, 0, NULL);
    hAverage = CreateThread(NULL, 0, AverageThread, NULL, 0, NULL);

    // Ожидание завершения потоков
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    // Замена минимальных и максимальных значений на среднее
    for (int& num : numbers) {
        if (num == min_value || num == max_value) {
            num = static_cast<int>(average_value); // Приводим average_value к int
        }
    }

    // Вывод полученного массива
    cout << "Modified array: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    // Завершение работы
    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    return 0;
}










