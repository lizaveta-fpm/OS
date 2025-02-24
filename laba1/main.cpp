#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <iomanip>

// Определение структуры Employee
struct Employee {
    int num;            // Идентификационный номер сотрудника
    char name[10];     // Имя сотрудника
    double hours;      // Количество отработанных часов
};

// Функция для запуска процесса
void RunProcess(const std::string& commandLine) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Создание процесса
    if (!CreateProcessA(
        NULL,
        const_cast<char*>(commandLine.c_str()),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
        return;
    }

    // Ожидание завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// Прототипы функций
void Creator(const std::string& binaryFilename, int numRecords);
void Reporter(const std::string& binaryFilename, const std::string& reportFilename, double hourlyRate);

int main() {
    std::string binaryFilename;
    int numRecords;

    std::cout << "Enter binary filename: ";
    std::cin >> binaryFilename;
    std::cout << "Enter number of records: ";
    std::cin >> numRecords;

    // Запуск утилиты Creator
    Creator(binaryFilename, numRecords);

    // Вывод содержимого бинарного файла
    std::ifstream infile(binaryFilename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file for reading." << std::endl;
        return 1;
    }

    std::cout << "\nContents of binary file:" << std::endl;
    Employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        std::cout << "  Number: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << std::endl;
    }
    infile.close();

    // Запрос данных для отчета
    std::string reportFilename;
    double hourlyRate;

    std::cout << "\nEnter report filename: ";
    std::cin >> reportFilename;
    std::cout << "Enter hourly rate: ";
    std::cin >> hourlyRate;

    // Запуск утилиты Reporter
    Reporter(binaryFilename, reportFilename, hourlyRate);

    // Вывод отчета на консоль
    std::ifstream reportFile(reportFilename);
    if (reportFile.is_open()) {
        std::cout << "\nReport contents:\n";
        std::string line;
        while (std::getline(reportFile, line)) {
            std::cout << line << std::endl;
        }
        reportFile.close();
    }
    else {
        std::cerr << "Unable to open report file." << std::endl;
        return 1;
    }

    std::cout << "Program finished." << std::endl;

    return 0;
}
