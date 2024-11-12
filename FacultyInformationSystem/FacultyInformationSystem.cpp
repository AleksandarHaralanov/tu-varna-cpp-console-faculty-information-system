#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

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

static void readFromFile(vector<Student>& students, int maxSize) {
    ifstream file("students.bin", ios::binary);
    if (!file.is_open()) return;

    students.clear();
    Student student;

    while (static_cast<int>(students.size()) < maxSize && file.peek() != EOF) {
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
    }

    file.close();
}

static void printHeader() {
    system("cls");
    cout << "--------------------------" << endl;
    cout << "Faculty Information System" << endl;
    cout << "--------------------------" << endl << endl;
}

static void printTableHeader() {
    cout << "+" << string(111, '-') << "+" << endl;
    cout << "|"
        << left << setw(35) << "Name" << "|"
        << left << setw(8) << "Major" << "|"
        << left << setw(8) << "Group" << "|"
        << left << setw(15) << "City" << "|"
        << left << setw(13) << "Faculty No." << "|"
        << left << setw(13) << "Birth Year" << "|"
        << left << setw(13) << "Adm. Score" << "|" << endl;
    cout << "+" << string(111, '-') << "+" << endl;
}

static void printStudentInformation(Student student) {
    cout << "|"
        << left << setw(35) << student.name << "|"
        << left << setw(8) << student.major << "|"
        << left << setw(8) << student.group << "|"
        << left << setw(15) << student.city << "|"
        << left << setw(13) << student.facultyNumber << "|"
        << left << setw(13) << student.birthYear << "|"
        << left << setw(13) << fixed << setprecision(2) << student.admissionScore
        << "|" << endl;
}

static bool inputInteger(int& val) {
    cin >> val;
    if (cin.fail() || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
        cout << endl << "Invalid input! Please enter an integer." << endl;
        system("pause");
        cout << endl;
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

static bool inputDouble(double& val) {
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << "Invalid input! Please enter a decimal number." << endl;
        system("pause");
        cout << endl;
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

static void addStudents(vector<Student>& students, int maxStudents) {
    int n = 0;

    while (true) {
        printHeader();
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

        printHeader();
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

    printHeader();
    printf("Successfully added %d student(s). You now have %d slots out of %d left.\n", n, maxStudents - static_cast<int>(students.size()), maxStudents);
    system("pause");
}

static void viewStudents(const vector<Student>& students) {
    printHeader();
    printTableHeader();

    for (const auto& student : students) {
        printStudentInformation(student);
    }

    cout << "+" << string(111, '-') << "+" << endl << endl;
    system("pause");
}

static void viewStudentHighestScore(const vector<Student>& students) {
    printHeader();
    printTableHeader();

    Student studentHighestScore;
    for (const auto& student : students) {
        if (student.admissionScore > studentHighestScore.admissionScore) studentHighestScore = student;
    }

    printStudentInformation(studentHighestScore);

    cout << "+" << string(111, '-') << "+" << endl << endl;
    system("pause");
}

static void viewStudentsVarna(const vector<Student>& students) {
    printHeader();
    printTableHeader();

    for (const auto& student : students) {
        if (student.city == "Varna") printStudentInformation(student);
    }

    cout << "+" << string(111, '-') << "+" << endl << endl;
    system("pause");
}

int main() {
    const int maxStudents = 150;
    vector<Student> students;
    readFromFile(students, maxStudents);

    int choice;
    while (true) {
        printHeader();
        cout << "1. Add students" << endl;
        cout << "2. View students" << endl;
        cout << "3. Sort students by faculty number ascending";
        cout << "0. Exit" << endl << endl;
        cout << "> ";

        if (!inputInteger(choice)) continue;

        switch (choice) {
        case 1:
            addStudents(students, maxStudents);
            break;
        case 2:
            do {
                printHeader();
                cout << "1. View all students" << endl;
                cout << "2. View student with highest admission score" << endl;
                cout << "3. View all students from Varna city" << endl;
                cout << "0. Go back" << endl << endl;
                cout << "> ";

                if (!inputInteger(choice)) continue;

                switch (choice) {
                case 1:
                    viewStudents(students);
                    break;
                case 2:
                    viewStudentHighestScore(students);
                    break;
                case 3:
                    viewStudentsVarna(students);
                    break;
                case 0:
                    break;
                default:
                    cout << endl << "Invalid input! Please enter an integer." << endl;
                    system("pause");
                    continue;
                }
            } while (choice != 0);
            break;
        case 3:
            sort(students.begin(), students.end(), [](const Student& a, const Student& b) { return a.facultyNumber < b.facultyNumber; });
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
