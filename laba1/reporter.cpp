#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

// Определение структуры Employee
struct Employee {
    int num;            // Идентификационный номер сотрудника
    char name[10];     // Имя сотрудника
    double hours;      // Количество отработанных часов
};

// Функция для создания отчета
void Reporter(const std::string& binaryFilename, const std::string& reportFilename, double hourlyRate) {
    std::ifstream infile(binaryFilename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening input file for reading." << std::endl;
        return;
    }

    std::ofstream outfile(reportFilename);
    if (!outfile) {
        std::cerr << "Error opening output file for writing." << std::endl;
        return;
    }

    outfile << "Report for file \"" << binaryFilename << "\"" << std::endl;
    outfile << "Employee Number, Employee Name, Hours, Salary" << std::endl;

    Employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        double salary = emp.hours * hourlyRate;
        outfile << emp.num << ", " << emp.name << ", " << emp.hours << ", " << std::fixed << std::setprecision(2) << salary << std::endl;
    }

    infile.close();
    outfile.close();
    std::cout << "Report created successfully: " << reportFilename << std::endl;
}
