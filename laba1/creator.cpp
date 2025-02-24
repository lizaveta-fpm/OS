#include <iostream>
#include <fstream>
#include <string>

// Определение структуры Employee
struct Employee {
    int num;            // Идентификационный номер сотрудника
    char name[10];     // Имя сотрудника
    double hours;      // Количество отработанных часов
};

// Функция для создания бинарного файла
void Creator(const std::string& binaryFilename, int numRecords) {
    std::ofstream outfile(binaryFilename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    Employee emp;
    for (int i = 0; i < numRecords; ++i) {
        std::cout << "Enter data for employee " << i + 1 << ":" << std::endl;
        std::cout << "  Number: ";
        std::cin >> emp.num;
        std::cout << "  Name: ";
        std::cin >> emp.name;
        std::cout << "  Hours: ";
        std::cin >> emp.hours;

        outfile.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    }

    outfile.close();
    std::cout << "Binary file created successfully: " << binaryFilename << std::endl;
}
