#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <cmath>

using namespace std;

struct Student {
    string major, group, name, city;
    int facultyNumber{}, birthYear{}, grades[6]{};
    double admissionScore{};
};

static void writeToFile(const Student students[], int studentCount) {
    ofstream file("..\\students.bin", ios::binary);
    if (file.is_open()) {
        for (int i = 0; i < studentCount; i++) {
            const Student& student = students[i];
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

            file.write(reinterpret_cast<const char*>(student.grades), sizeof(student.grades));
        }
        file.close();
    }
}

static void readFromFile(Student students[], int& studentCount, int maxSize) {
    ifstream file("..\\students.bin", ios::binary);
    if (!file.is_open()) {
        studentCount = 0;
        return;
    }

    studentCount = 0;
    while (studentCount < maxSize && file.peek() != EOF) {
        Student student;
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

        file.read(reinterpret_cast<char*>(student.grades), sizeof(student.grades));

        students[studentCount++] = student;
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
    cout << "+" << string(125, '-') << "+" << endl;
    cout << "|"
        << left << setw(35) << "Name" << "|"
        << left << setw(15) << "City" << "|"
        << left << setw(13) << "Birth Year" << "|"
        << left << setw(8) << "Major" << "|"
        << left << setw(8) << "Group" << "|"
        << left << setw(14) << "Faculty No." << "|"
        << left << setw(13) << "Adm. Score" << "|"
        << left << setw(12) << "Grades" << "|" << endl;
    cout << "+" << string(125, '-') << "+" << endl;
}

static void printTableEnd() {
    cout << "+" << string(125, '-') << "+" << endl << endl;
    system("pause");
}

static void printStudentInformation(const Student& student) {
    cout << "|"
        << left << setw(35) << student.name << "|"
        << left << setw(15) << student.city << "|"
        << left << setw(13) << student.birthYear << "|"
        << left << setw(8) << student.major << "|"
        << left << setw(8) << student.group << "|"
        << left << setw(14) << student.facultyNumber << "|"
        << left << setw(13) << fixed << setprecision(2) << student.admissionScore << "|";

    for (int i = 0; i < 6; i++)
        cout << right << setw(1) << student.grades[i];

    cout << "      |" << endl;
}

static bool inputInteger(int& val) {
    cin >> val;

    if (cin.fail() || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << "Invalid input! Please enter an integer." << endl;
        system("pause");
        cout << endl;
        return false;
    }

    if (val < 0) {
        cout << endl << "Invalid input! The number must be 0 or above." << endl;
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

    if (val < 0.0) {
        cout << endl << "Invalid input! The number must be 0 or above." << endl;
        system("pause");
        cout << endl;
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

static void addStudents(Student students[], int& studentCount, int maxStudents) {
    int n;
    while (true) {
        printHeader();
        printf("(%d out of %d slots left)\n\n", maxStudents - studentCount, maxStudents);
        cout << "How many students do you wish to add?" << endl;
        cout << "(Enter 0 to go back) > ";

        if (inputInteger(n) && n >= 0) {
            if (n == 0) return;
            else if (studentCount + n <= maxStudents) break;
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
        printf("Student %d Information\n\n", studentCount + 1);

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
            if (inputInteger(student.facultyNumber)) {
                if (to_string(student.facultyNumber).length() != 8) {
                    cout << endl << "Invalid input! Faculty numbers must be exactly 8 digits long." << endl << endl;
                    continue;
                }

                bool duplicate = false;
                for (int j = 0; j < studentCount; j++)
                    if (students[j].facultyNumber == student.facultyNumber) {
                        cout << endl << "This faculty number already exists. Please enter a unique faculty number." << endl << endl;
                        duplicate = true;
                        break;
                    }

                if (!duplicate) break;
            }
        }

        cout << "Major > ";
        getline(cin, student.major);

        cout << "Group > ";
        getline(cin, student.group);

        students[studentCount++] = student;
    }

    printHeader();
    printf("Successfully added %d student(s). You now have %d slots out of %d left.\n", n, maxStudents - studentCount, maxStudents);
    system("pause");
}

static void viewStudents(const Student students[], int studentCount) {
    printHeader();
    printTableHeader();

    for (int i = 0; i < studentCount; i++) printStudentInformation(students[i]);

    printTableEnd();
}

static void viewStudentHighestScore(const Student students[], int studentCount) {
    printHeader();
    printTableHeader();

    if (studentCount == 0) {
        cout << "|No students available." << setw(103) << " " << "|" << endl;
    }
    else {
        Student studentHighestScore = students[0];
        for (int i = 1; i < studentCount; i++)
            if (students[i].admissionScore > studentHighestScore.admissionScore)
                studentHighestScore = students[i];

        printStudentInformation(studentHighestScore);
    }

    printTableEnd();
}

static void viewStudentsVarna(const Student students[], int studentCount) {
    printHeader();
    printTableHeader();

    bool isFound = false;
    for (int i = 0; i < studentCount; i++)
        if (students[i].city == "Varna") {
            printStudentInformation(students[i]);
            isFound = true;
        }

    if (!isFound) cout << "|No students from Varna found." << setw(96) << " " << "|" << endl;

    printTableEnd();
}

static void searchStudentsGivenGroupScoreDesc(const Student students[], int studentCount) {
    printHeader();

    string group;
    cout << "Group > ";
    getline(cin, group);

    Student groupStudents[150];
    int groupStudentCount = 0;

    for (int i = 0; i < studentCount; i++)
        if (students[i].group == group)
            groupStudents[groupStudentCount++] = students[i];

    printHeader();
    printf("Students in group '%s' sorted by admission score descending\n\n", group.c_str());
    printTableHeader();

    if (groupStudentCount == 0) {
        cout << "|No students found matching the criteria." << setw(85) << " " << "|" << endl;
        printTableEnd();
        return;
    }

    sort(groupStudents, groupStudents + groupStudentCount, [](const Student& a, const Student& b) { return a.admissionScore > b.admissionScore; });

    for (int i = 0; i < groupStudentCount; i++)
        printStudentInformation(groupStudents[i]);

    printTableEnd();
}

static void searchStudentsGivenMajorGroup(const Student students[], int studentCount) {
    printHeader();

    string major, group;

    cout << "Major > ";
    getline(cin, major);

    cout << "Group > ";
    getline(cin, group);

    printHeader();
    printf("Students in major '%s' and group '%s'\n\n", major.c_str(), group.c_str());
    printTableHeader();

    bool isFound = false;
    for (int i = 0; i < studentCount; i++)
        if (students[i].major == major && students[i].group == group) {
            printStudentInformation(students[i]);
            isFound = true;
        }

    if (!isFound) cout << "|No students found matching the criteria." << setw(85) << " " << "|" << endl;

    printTableEnd();
}

static void studentGradeEntry(Student students[], int studentCount) {
    printHeader();

    int facultyNumber;
    while (true) {
        cout << "Faculty number > ";
        if (inputInteger(facultyNumber)) {
            if (to_string(facultyNumber).length() != 8) {
                cout << endl << "Invalid input! Faculty numbers must be exactly 8 digits long." << endl << endl;
                continue;
            }
            break;
        }
    }

    bool isFound = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].facultyNumber != facultyNumber) continue;
        isFound = true;

        printHeader();
        printf("Performing grade entries for %s with Fac. No %d\n\n", students[i].name.c_str(), students[i].facultyNumber);

        cout << "Enter grades for 6 subjects:\n";
        for (int j = 0; j < 6; j++) {
            while (true) {
                cout << "Grade for subject " << (j + 1) << " > ";
                if (inputInteger(students[i].grades[j])) {
                    if (students[i].grades[j] >= 2 && students[i].grades[j] <= 6) break;
                    else cout << endl << "Invalid input! Grade must be between 2 and 6." << endl << endl;
                }
            }
        }

        cout << endl << "Grades entered successfully!" << endl;
        system("pause");
        break;
    }

    if (!isFound) {
        printf("\nCould not find a student with a Fac. No %d\n\n", facultyNumber);
        system("pause");
    }
}

static void checkScholarshipEligibility(const Student students[], int studentCount) {
    printHeader();

    int facultyNumber;
    while (true) {
        cout << "Faculty number > ";
        if (inputInteger(facultyNumber)) {
            if (to_string(facultyNumber).length() != 8) {
                cout << endl << "Invalid input! Faculty numbers must be exactly 8 digits long." << endl << endl;
                continue;
            }
            break;
        }
    }

    Student student;
    bool isYoung = true;
    bool isPassing = true;
    bool isFound = false;
    double averageGrade = 0;
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].facultyNumber != facultyNumber) continue;

        student = students[i];
        isFound = true;
        isYoung = (2022 - students[i].birthYear) < 26;
        for (int j = 0; j < 6; j++) {
            if (students[i].grades[j] >= 3) {
                averageGrade += students[i].grades[j];
                continue;
            }
            isPassing = false;
            break;
        }
        
        if (isPassing) averageGrade /= 6;

        break;
    }

    if (isFound) {
        if (!isPassing) printf("\n%s is not eligible to apply for a scholarship due to failing or unpassed exams.\n(Grades: %d %d %d %d %d %d)\n\n",
            student.name.c_str(), student.grades[0], student.grades[1], student.grades[2], student.grades[3], student.grades[4], student.grades[5]);
        else if (!isYoung) printf("\n%s exceeds the age limit for eligibility.\nThey are %d years old right now. Requirement is to be under 26 years old.\n\n",
            student.name.c_str(), 2022 - student.birthYear);
        else printf("\n%s application score for the scholarship is '%.2f'.\n\n", student.name.c_str(), averageGrade);
    }
    else cout << endl << "Student with Fac. No '" << facultyNumber << "' not found." << endl << endl;

    system("pause");
}

int main() {
    const int maxStudents = 150;
    Student students[maxStudents];
    int studentCount = 0;

    readFromFile(students, studentCount, maxStudents);

    int choice;
    while (true) {
        printHeader();
        cout << "1. Add students" << endl;
        cout << "2. View students" << endl;
        cout << "3. Search students" << endl;
        cout << "4. Perform student grade entries" << endl;
        cout << "5. Check scholarship eligibility" << endl;
        cout << "6. Sort students by faculty number ascending" << endl << endl;
        cout << "0. Exit" << endl << endl;
        cout << "> ";

        if (!inputInteger(choice)) continue;

        switch (choice) {
        case 1:
            addStudents(students, studentCount, maxStudents);
            break;
        case 2:
            do {
                printHeader();
                cout << "1. View all students" << endl;
                cout << "2. View student with highest admission score" << endl;
                cout << "3. View all students from Varna city" << endl << endl;
                cout << "0. Go back" << endl << endl;
                cout << "> ";

                if (!inputInteger(choice)) continue;

                switch (choice) {
                case 1:
                    viewStudents(students, studentCount);
                    break;
                case 2:
                    viewStudentHighestScore(students, studentCount);
                    break;
                case 3:
                    viewStudentsVarna(students, studentCount);
                    break;
                case 0:
                    break;
                default:
                    cout << endl << "Invalid input! Please enter a valid option." << endl;
                    system("pause");
                    continue;
                }
            } while (choice != 0);
            break;
        case 3:
            do {
                printHeader();
                cout << "1. Search students in given group by admission score descending" << endl;
                cout << "2. Search students in given major and group" << endl << endl;
                cout << "0. Go back" << endl << endl;
                cout << "> ";

                if (!inputInteger(choice)) continue;

                switch (choice) {
                case 1:
                    searchStudentsGivenGroupScoreDesc(students, studentCount);
                    break;
                case 2:
                    searchStudentsGivenMajorGroup(students, studentCount);
                    break;
                case 0:
                    break;
                default:
                    cout << endl << "Invalid input! Please enter a valid option." << endl;
                    system("pause");
                    continue;
                }
            } while (choice != 0);
            break;
        case 4:
            studentGradeEntry(students, studentCount);
            break;
        case 5:
            checkScholarshipEligibility(students, studentCount);
            break;
        case 6:
            sort(students, students + studentCount, [](const Student& a, const Student& b) { return a.facultyNumber < b.facultyNumber; });
            printHeader();
            cout << "Sorted all students by faculty number in ascending order." << endl << endl;
            system("pause");
            break;
        case 0:
            writeToFile(students, studentCount);
            printHeader();
            cout << "Saved and backed up all student data to a binary file in program directory. See 'students.bin' for reference." << endl;
            return 0;
        default:
            cout << endl << "Invalid input! Please enter a valid option." << endl;
            system("pause");
            break;
        }
    }
}
