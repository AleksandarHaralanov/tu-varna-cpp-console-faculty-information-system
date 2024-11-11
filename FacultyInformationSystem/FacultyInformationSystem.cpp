#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct Student {
    string major, group, name, city;
    int facultyNumber{}, birthYear{};
    double admissionScore{};
};

static void writeToFile(Student students[], int size) {
    ofstream file("students.bin", ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(students), sizeof(Student) * size);
        file.close();
    }
}

static int readFromFile(Student students[], int maxSize) {
    ifstream file("students.bin", ios::binary);
    if (!file.is_open()) return 0;

    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int count = static_cast<int>(fileSize / sizeof(Student));
    if (count > maxSize) count = maxSize;

    file.read(reinterpret_cast<char*>(students), sizeof(Student) * count);
    file.close();

    return count;
}

static bool inputInteger(int& val) {
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << "Invalid input! Please enter a number." << endl;
        system("pause");
        return false;
    }
    return true;
}

static void addStudents(Student students[], int& count, int maxStudents) {
    int n = 0;

    while (true) {
        system("cls");
        cout << "Faculty Information System" << endl << endl;
        cout << "How many students do you wish to add?" << endl;
        printf("(%d out of %d slots left) > ", maxStudents - count, maxStudents);

        if (inputInteger(n) && n >= 0) {
            if (n == 0) return;
            else if (count + n <= maxStudents) {
                cin.ignore();
                break;
            }
            else {
                cout << endl << "You don't have enough slots to add this many students." << endl;
                system("pause");
                continue;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        system("cls");
        cout << "Full name > ";
        getline(cin, students[i].name);

        cout << endl << "City > ";
        getline(cin, students[i].city);

        cout << endl << "Birth year > ";
        cin >> students[i].birthYear;
        cin.ignore();

        cout << endl << "Admission score > ";
        cin >> students[i].admissionScore;
        cin.ignore();

        cout << endl << "Faculty number > ";
        cin >> students[i].facultyNumber;
        cin.ignore();

        cout << endl << "Major > ";
        getline(cin, students[i].major);

        cout << endl << "Group > ";
        getline(cin, students[i].group);

        count++;
    }

    system("cls");
    printf("Successfully added %d students. You now have %d slots out of a %d left.\n", n, maxStudents - count, maxStudents);
    system("pause");
}

int main() {
    const int maxStudents = 150;
    Student students[maxStudents] = {};
    int count = readFromFile(students, maxStudents);

    int choice;
    while (true) {
        system("cls");
        cout << "Faculty Information System" << endl << endl;
        cout << "1. Add students" << endl << endl;
        cout << "> ";

        if (!inputInteger(choice)) continue;

        switch (choice) {
        case 1:
            addStudents(students, count, maxStudents);
            break;
        default:
            return 0;
        }
    }
}
