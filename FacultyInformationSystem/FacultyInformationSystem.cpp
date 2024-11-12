#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct Student {
    string major, group, name, city;
    int facultyNumber{}, birthYear{};
    double admissionScore{};
};

static void writeToFile(const vector<Student>& students) {
    ofstream file("students.bin", ios::binary);
    if (file.is_open()) {
        for (const auto& student : students) {
            int length;

            length = static_cast<int>(student.name.size());
            file.write(reinterpret_cast<const char*>(&length), sizeof(length));
            file.write(student.name.c_str(), length);

            length = static_cast<int>(student.city.size());
            file.write(reinterpret_cast<const char*>(&length), sizeof(length));
            file.write(student.city.c_str(), length);

            length = static_cast<int>(student.major.size());
            file.write(reinterpret_cast<const char*>(&length), sizeof(length));
            file.write(student.major.c_str(), length);

            length = static_cast<int>(student.group.size());
            file.write(reinterpret_cast<const char*>(&length), sizeof(length));
            file.write(student.group.c_str(), length);

            file.write(reinterpret_cast<const char*>(&student.facultyNumber), sizeof(student.facultyNumber));
            file.write(reinterpret_cast<const char*>(&student.birthYear), sizeof(student.birthYear));
            file.write(reinterpret_cast<const char*>(&student.admissionScore), sizeof(student.admissionScore));
        }
        file.close();
    }
}

static int readFromFile(vector<Student>& students, int maxSize) {
    ifstream file("students.bin", ios::binary);
    if (!file.is_open()) return 0;

    students.clear();
    Student student;
    int count = 0;

    while (count < maxSize && file.peek() != EOF) {
        int length = 0;

        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        student.name.resize(length);
        file.read(&student.name[0], length);

        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        student.city.resize(length);
        file.read(&student.city[0], length);

        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        student.major.resize(length);
        file.read(&student.major[0], length);

        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        student.group.resize(length);
        file.read(&student.group[0], length);

        file.read(reinterpret_cast<char*>(&student.facultyNumber), sizeof(student.facultyNumber));
        file.read(reinterpret_cast<char*>(&student.birthYear), sizeof(student.birthYear));
        file.read(reinterpret_cast<char*>(&student.admissionScore), sizeof(student.admissionScore));

        students.push_back(student);
        ++count;
    }

    file.close();
    return count;
}

static bool inputInteger(int& val) {
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter an integer." << endl;
        system("pause");
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

static bool inputDouble(double& val) {
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a decimal number." << endl;
        system("pause");
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

static void addStudents(vector<Student>& students, int maxStudents) {
    int n = 0;

    while (true) {
        system("cls");
        cout << "Faculty Information System" << endl << endl;
        cout << "How many students do you wish to add?" << endl;
        printf("(%d out of %d slots left) > ", maxStudents - static_cast<int>(students.size()), maxStudents);

        if (inputInteger(n) && n >= 0) {
            if (n == 0) return;
            else if (static_cast<int>(students.size()) + n <= maxStudents) break;
            else {
                cout << "You don't have enough slots to add this many students." << endl;
                system("pause");
                continue;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        Student student;

        system("cls");
        cout << "Faculty Information System" << endl << endl;
        printf("Student %d Information\n\n", static_cast<int>(students.size()) + 1);

        cout << "Full name > ";
        getline(cin, student.name);

        cout << "City > ";
        getline(cin, student.city);

        while (true) {
            cout << "Birth year > ";
            if (inputInteger(student.birthYear)) break;
        }

        while (true) {
            cout << "Admission score > ";
            if (inputDouble(student.admissionScore)) break;
        }

        while (true) {
            cout << "Faculty number > ";
            if (inputInteger(student.facultyNumber)) break;
        }

        cout << "Major > ";
        getline(cin, student.major);

        cout << "Group > ";
        getline(cin, student.group);

        students.push_back(student);
    }

    system("cls");
    printf("Successfully added %d students. You now have %d slots out of %d left.\n", n, maxStudents - static_cast<int>(students.size()), maxStudents);
    system("pause");
}

int main() {
    const int maxStudents = 150;
    vector<Student> students;
    int count = readFromFile(students, maxStudents);

    int choice;
    while (true) {
        system("cls");
        cout << "Faculty Information System" << endl << endl;
        cout << "1. Add students" << endl;
        cout << "0. Exit" << endl << endl;
        cout << "> ";

        if (!inputInteger(choice)) continue;

        switch (choice) {
        case 1:
            addStudents(students, maxStudents);
            break;
        case 0:
            writeToFile(students);
            return 0;
        default:
            cout << endl << "Invalid input! Please enter an integer." << endl;
            system("pause");
            break;
        }
    }
}
