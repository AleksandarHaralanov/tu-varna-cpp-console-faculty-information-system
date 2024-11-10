#include <iostream>
#include <fstream>

using namespace std;

struct Student {
    char major, group, name, city;
    int facultyNumber, birthYear;
    double admissionScore;
};

static void writeToFile(Student students[], int size)
{
    ofstream file("students.bin", ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<char*>(students), sizeof(Student) * size);
        file.close();
    }
}

static int readFromFile(Student students[], int maxSize)
{
    ifstream file("students.bin", ios::binary);
    if (!file.is_open()) return 0;

    file.seekg(0, ios::end);
    long fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int count = fileSize / sizeof(Student);
    if (count > maxSize) count = maxSize;

    file.read(reinterpret_cast<char*>(students), sizeof(Student) * count);
    file.close();

    return count;
}

int main()
{
    Student students[150] = {};
    int count = readFromFile(students, 150);

    return 0;
}
