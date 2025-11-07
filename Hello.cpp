#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


class Student {
private:
    int roll;
    char name[50];
    int marks[5];      
    float percentage;
    char grade;

public:
    
    Student() {
        roll = 0;
        percentage = 0.0;
        grade = 'F';
    }

    
    ~Student() {}

    
    void input() {
        cout << "\nEnter Roll No: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Name: ";
        cin.getline(name, 50);
        cout << "Enter marks in 5 subjects: ";
        int total = 0;
        for (int i = 0; i < 5; i++) {
            cin >> marks[i];
            total += marks[i];
        }
        percentage = total / 5.0;
        calculate();
    }

    
    virtual void calculate() {
        if (percentage >= 90) grade = 'A';
        else if (percentage >= 75) grade = 'B';
        else if (percentage >= 60) grade = 'C';
        else if (percentage >= 50) grade = 'D';
        else grade = 'F';
    }

  
    virtual void display() const {
        cout << "\nRoll: " << roll;
        cout << "\nName: " << name;
        cout << "\nPercentage: " << percentage;
        cout << "\nGrade: " << grade << "\n";
    }

    
    int getRoll() const { return roll; }

    
    friend void showPercentage(const Student &s);

    Student operator+(const Student &other) {
        Student temp;
        temp.roll = -1; 
        strcpy(temp.name, "Combined");
        for (int i = 0; i < 5; i++) {
            temp.marks[i] = this->marks[i] + other.marks[i];
        }
        int total = 0;
        for (int i = 0; i < 5; i++) total += temp.marks[i];
        temp.percentage = total / 5.0;
        temp.calculate();
        return temp;
    }
};


void showPercentage(const Student &s) {
    cout << "\n Percentage of student is: " << s.percentage << "%\n";
}


void addStudent() {
    Student s;
    ofstream fout("students.dat", ios::binary | ios::app);
    s.input();
    fout.write((char*)&s, sizeof(s));
    fout.close();
    cout << "\nRecord Added Successfully!\n";
}

void displayAll() {
    Student s;
    ifstream fin("students.dat", ios::binary);
    if (!fin) {
        cout << "\nNo records found!\n";
        return;
    }
    cout << "\n==== All Student Records ====\n";
    while (fin.read((char*)&s, sizeof(s))) {
        s.display();
    }
    fin.close();
}

void searchStudent(int roll) {
    Student s;
    ifstream fin("students.dat", ios::binary);
    bool found = false;
    while (fin.read((char*)&s, sizeof(s))) {
        if (s.getRoll() == roll) {
            cout << "\n==== Student Found ====\n";
            s.display();
            showPercentage(s); 
            found = true;
        }
    }
    fin.close();
    if (!found) cout << "\nStudent with Roll " << roll << " not found!\n";
}

void deleteStudent(int roll) {
    Student s;
    ifstream fin("students.dat", ios::binary);
    ofstream fout("temp.dat", ios::binary);
    bool deleted = false;

    while (fin.read((char*)&s, sizeof(s))) {
        if (s.getRoll() != roll) {
            fout.write((char*)&s, sizeof(s));
        } else {
            deleted = true;
        }
    }
    fin.close();
    fout.close();
    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (deleted) cout << "\nRecord Deleted Successfully!\n";
    else cout << "\nRecord not found!\n";
}

void modifyStudent(int roll) {
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    Student s;
    bool found = false;

    while (file.read((char*)&s, sizeof(s)) && !found) {
        if (s.getRoll() == roll) {
            cout << "\nEnter new details for Roll " << roll << ":\n";
            s.input();
            int pos = -1 * sizeof(s);
            file.seekp(pos, ios::cur);
            file.write((char*)&s, sizeof(s));
            cout << "\nRecord Updated Successfully!\n";
            found = true;
        }
    }
    file.close();
    if (!found) cout << "\nRecord not found!\n";
}


int main() {
    int choice, roll;
    do {
        cout << "\n========= Student Report Card System =========";
        cout << "\n1. Add Student";
        cout << "\n2. Display All Students";
        cout << "\n3. Search Student";
        cout << "\n4. Modify Student";
        cout << "\n5. Delete Student";
        cout << "\n6. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: cout << "Enter Roll No to Search: "; cin >> roll; searchStudent(roll); break;
            case 4: cout << "Enter Roll No to Modify: "; cin >> roll; modifyStudent(roll); break;
            case 5: cout << "Enter Roll No to Delete: "; cin >> roll; deleteStudent(roll); break;
            case 6: cout << "\nExiting Program...\n"; break;
            default: cout << "\nInvalid Choice!\n";
        }
    } while (choice != 6);

return 0;
}
