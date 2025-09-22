#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

// ------- Helper Functions -------
void ignoreLine() { cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
bool isAlphaString(const string &s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.length(); i++)
        if (!isalpha(s[i]) && s[i] != ' ') return false;
    return true;
}
bool isValidEmail(const string& email) {
    int at = email.find("@");
    int dot = email.find(".", at);
    return at > 0 && dot > at + 1 && dot < static_cast<int>(email.length())-1;
}
string inputOrSkip(const string& prompt, const string& current) {
    cout << prompt << " (" << current << "): ";
    string s; getline(cin, s);
    return s.empty() ? current : s;
}
string toLower(const string& s) {
    string r = s;
    for(size_t i=0; i<r.size(); ++i) r[i]=tolower(r[i]);
    return r;
}
struct CompareMarks {
    bool operator()(const pair<string,int>& a, const pair<string,int>& b) const {
        return a.second > b.second;
    }
};
struct CompareGPA {
    bool operator()(const pair<string,float>& a, const pair<string,float>& b) const {
        return a.second > b.second;
    }
};

// --------- User Classes ----------
class User {
protected:
    string name;
    int id;
    string email;
    string password;
public:
    User() : id(0) {}
    virtual void input() {
        do {
            cout << "Enter Name: ";
            getline(cin, name);
        } while (!isAlphaString(name));
        cout << "Enter ID: "; cin >> id; ignoreLine();
        do {
            cout << "Enter Email: ";
            getline(cin, email);
            if(!isValidEmail(email)) cout << "Invalid email, try again.\n";
        } while (!isValidEmail(email));
        cout << "Enter Password: "; getline(cin, password);
    }
    virtual void update() {
        name = inputOrSkip("Update Name", name);
        email = inputOrSkip("Update Email", email);
        password = inputOrSkip("Update Password", password);
    }
    virtual void display() const {
        cout << left << setw(15) << "Name:" << name << "\n"
             << left << setw(15) << "ID:" << id << "\n"
             << left << setw(15) << "Email:" << email << endl;
    }
    int getID() const { return id; }
    string getName() const { return name; }
};
class Teacher : public User {
public:
    Teacher() {}
    virtual void display() const {
        cout << left << setw(15) << "Teacher:" << name << " | ID: " << id << " | Email: " << email << endl;
    }
};
class Student : public User {
private:
    map<int, int> courseMarks;
public:
    void enrollCourse(int cid) { courseMarks[cid] = 0; }
    void setMarks(int cid, int marks) { courseMarks[cid] = marks; }
    int getMarks(int cid) const {
        map<int, int>::const_iterator it = courseMarks.find(cid);
        return (it != courseMarks.end()) ? it->second : -1;
    }
    float calculateGPA(const map<int,int>& courseCredits) const {
        float totalPoints = 0;
        int totalCredits = 0;
        for (map<int,int>::const_iterator it = courseMarks.begin(); it != courseMarks.end(); ++it) {
            int cid = it->first;
            int marks = it->second;
            map<int,int>::const_iterator cc = courseCredits.find(cid);
            if (cc != courseCredits.end()) {
                int cr = cc->second;
                totalPoints += marks * cr;
                totalCredits += cr;
            }
        }
        return (totalCredits > 0) ? totalPoints / totalCredits / 25.0f : 0;
    }
    void display() const {
        User::display();
        cout << left << setw(15) << "Courses:";
        if(courseMarks.empty()) cout << "None" << endl;
        else {
            cout << endl;
            cout << setw(10) << "CourseID" << setw(10) << "Marks" << endl;
            cout << "----------------------" << endl;
            map<int,int>::const_iterator it = courseMarks.begin();
            for(; it != courseMarks.end(); ++it)
                cout << setw(10) << it->first << setw(10) << it->second << endl;
        }
    }
    void update() { User::update(); }
};
class Course {
private:
    int courseID;
    string courseName;
    int creditHours;
    int teacherID;
public:
    Course(): courseID(0), creditHours(0), teacherID(0) {}
    void input() {
        cout << "Enter Course ID: "; cin >> courseID; ignoreLine();
        do {
            cout << "Enter Course Name: "; getline(cin, courseName);
        } while(!isAlphaString(courseName));
        cout << "Enter Credit Hours: "; cin >> creditHours; ignoreLine();
        cout << "Assign Teacher by ID: "; cin >> teacherID; ignoreLine();
    }
    void display() const {
        cout << setw(10) << courseID << setw(25) << courseName
             << setw(10) << creditHours << setw(10) << teacherID << endl;
    }
    int getID() const { return courseID; }
    string getName() const { return courseName; }
    int getCredits() const { return creditHours; }
    int getTeacher() const { return teacherID; }
    void setTeacher(int tid) { teacherID = tid; }
    void update() {
        string newName;
        int newCredits, newTeacher;
        cout << "Update Name (" << courseName << "): "; getline(cin, newName); if(isAlphaString(newName)) courseName = newName;
        cout << "Update Credits (" << creditHours << "): "; cin >> newCredits; ignoreLine();
        if(newCredits>0) creditHours=newCredits;
        cout << "Update Teacher ID (" << teacherID << "): "; cin >> newTeacher; ignoreLine();
        teacherID = newTeacher;
    }
};
// ----------- LMS Main Class -----------
class LMS {
private:
    vector<Student> students;
    vector<Course> courses;
    vector<Teacher> teachers;
    map<int,int> courseCredits;
public:
    void addStudent() {
        Student s; s.input();
        for (size_t i = 0; i < students.size(); i++)
            if (students[i].getID() == s.getID()) { cout << "Duplicate ID.\n"; return; }
        students.push_back(s);
        cout << "Student added!\n";
    }
    void bulkAddStudents() {
        cout<<"How many students to add? "; int n; cin>>n; ignoreLine();
        for(int i=0;i<n;++i) {
            cout<<"\nAdding student "<<i+1<<"/"<<n<<":\n";
            addStudent();
        }
    }
    void viewStudents() {
        if(students.empty()){ cout<<"No students.\n"; return;}
        for (size_t i = 0; i < students.size(); i++)
            students[i].display();
    }
    void searchStudent() {
        cout << "Enter Student Name or ID: ";
        string key;
        getline(cin, key);

        bool found = false, isId = true;
        for(size_t i=0;i<key.size();++i)
            if (!isdigit(key[i])) { isId = false; break; }

        if(isId && !key.empty()) {
            int id = atoi(key.c_str());
            for (size_t i = 0; i < students.size(); ++i)
                if (students[i].getID() == id) { students[i].display(); found = true; }
        }
        string keyL = toLower(key);
        for (size_t i = 0; i < students.size(); ++i)
            if (toLower(students[i].getName()) == keyL) { students[i].display(); found = true; }
        if(!found) cout << "Not found.\n";
    }
    void searchStudentPartial() {
        cout << "Enter Name fragment (case-insensitive): "; string key; getline(cin, key);
        string keyL = toLower(key);
        bool found = false;
        for (size_t i = 0; i < students.size(); i++) {
            string nameL = toLower(students[i].getName());
            if(nameL.find(keyL) != string::npos) {
                students[i].display(); found = true;
            }
        }
        if(!found) cout<<"No student found with fragment.\n";
    }
    void updateStudent() {
        cout << "Enter Student ID to update: ";
        int sid; cin >> sid; ignoreLine();
        for(size_t i = 0; i < students.size(); ++i)
            if(students[i].getID() == sid) {
                cout << "Current info:\n"; students[i].display();
                cout << "Enter new info (leave blank to keep unchanged):\n";
                students[i].update();
                cout << "Updated!\n"; return;
            }
        cout << "Student not found.\n";
    }
    void enrollStudentInCourse(int sid, int cid) {
        Student* st = NULL;
        for (size_t i = 0; i < students.size(); i++)
            if (students[i].getID() == sid) { st = &students[i]; break; }
        bool foundCourse = false;
        for (size_t i = 0; i < courses.size(); i++)
            if (courses[i].getID() == cid) { foundCourse = true; break; }
        if (!st || !foundCourse) { cout << "Invalid Student or Course ID.\n"; return; }
        st->enrollCourse(cid);
        cout << "Student enrolled!\n";
    }
    void addTeacher() {
        Teacher t; t.input();
        for(size_t i = 0; i < teachers.size(); ++i)
            if(teachers[i].getID() == t.getID()) { cout << "Duplicate Teacher ID.\n"; return; }
        teachers.push_back(t);
        cout << "Teacher added!\n";
    }
    void viewTeachers() {
        if(teachers.empty()) { cout << "No teachers.\n"; return; }
        for(size_t i=0; i < teachers.size(); ++i)
            teachers[i].display();
    }
    void updateTeacher() {
        cout << "Enter Teacher ID to update: "; int tid; cin >> tid; ignoreLine();
        for(size_t i=0; i<teachers.size(); ++i)
            if(teachers[i].getID() == tid) {
                cout << "Current info:\n"; teachers[i].display();
                cout << "Enter new info (blank to keep unchanged):\n";
                teachers[i].update();
                cout << "Teacher updated!\n"; return;
            }
        cout << "Teacher not found.\n";
    }
    void addCourse() {
        Course c; c.input();
        courses.push_back(c);
        courseCredits[c.getID()] = c.getCredits();
        cout << "Course added!\n";
    }
    void bulkAddCourses() {
        cout<<"How many courses to add? "; int n; cin>>n; ignoreLine();
        for(int i=0;i<n;++i) {
            cout<<"\nAdding course "<<i+1<<"/"<<n<<":\n";
            addCourse();
        }
    }
    void viewCourses() {
        if(courses.empty()){ cout<<"No courses.\n"; return;}
        cout << setw(10) << "CourseID" << setw(25) << "Name" << setw(10) << "Credits" << setw(10) << "TeacherID" << endl;
        cout << "--------------------------------------------------------------\n";
        for (size_t i = 0; i < courses.size(); i++)
            courses[i].display();
    }
    void updateCourse() {
        cout << "Enter Course ID to update: "; int cid; cin >> cid; ignoreLine();
        for(size_t i=0;i<courses.size();++i)
            if(courses[i].getID() == cid) {
                cout<<"Current course info:\n"; courses[i].display();
                cout<<"Enter new info (blank/0 to skip):\n"; courses[i].update();
                courseCredits[courses[i].getID()] = courses[i].getCredits();
                cout<<"Course updated!\n"; return;
            }
        cout<<"Course not found.\n";
    }
    void setStudentMarks() {
        cout << "Enter Student ID: "; int sid; cin >> sid; ignoreLine();
        cout << "Enter Course ID: "; int cid; cin >> cid; ignoreLine();
        cout << "Enter Marks (0-100): "; int marks; cin >> marks; ignoreLine();
        if(marks<0 || marks>100) { cout<<"Marks range error.\n"; return;}
        for(size_t i=0;i<students.size();++i)
            if(students[i].getID() == sid) {
                students[i].setMarks(cid, marks); cout<<"Marks updated!\n"; return;
            }
        cout<<"Student not found.\n";
    }
    void displayStudentGPA(int sid) {
        Student* st = NULL;
        for (size_t i = 0; i < students.size(); i++)
            if (students[i].getID() == sid) { st = &students[i]; break; }
        if (!st) { cout << "Student not found.\n"; return; }
        float gpa = st->calculateGPA(courseCredits);
        cout << st->getName() << "'s CGPA: " << fixed << setprecision(2) << gpa << endl;
    }
    void rankStudentsInCourse(int cid) {
        vector< pair<string, int> > ranking;
        for(size_t i=0;i<students.size();++i)
            if(students[i].getMarks(cid) >= 0)
                ranking.push_back( pair<string,int>(students[i].getName(), students[i].getMarks(cid)) );
        if(ranking.empty()) { cout<<"No students for course "<<cid<<endl; return; }
        sort(ranking.begin(), ranking.end(), CompareMarks());
        cout << "Ranking for Course "<< cid <<":\n";
        for(size_t i=0;i<ranking.size();++i)
            cout<<i+1<<". "<<ranking[i].first<<" - Marks: "<<ranking[i].second<<endl;
    }
    void rankAllStudentsByGPA() {
        vector< pair<string, float> > ranking;
        for(size_t i=0;i<students.size();++i)
            ranking.push_back( pair<string,float>(students[i].getName(), students[i].calculateGPA(courseCredits)) );
        sort(ranking.begin(), ranking.end(), CompareGPA());
        cout<<"Ranking All Students by GPA:\n";
        for(size_t i=0;i<ranking.size();++i)
            cout<<i+1<<". "<<ranking[i].first<<" - GPA: "<<fixed<<setprecision(2)<<ranking[i].second<<endl;
    }
};

// ----------------- Submenus -----------------
void studentMenu(LMS &lms) {
    int choice;
    do {
        cout<<"\n--- STUDENT MENU ---\n";
        cout<<"1. Add Student\n2. View Students\n3. Search Student\n4. Update Student\n";
        cout<<"5. Enroll Student in Course\n6. View Student Dashboard\n";
        cout<<"7. Partial Name Search\n8. Bulk Add Students\n0. Back\nChoice: ";
        cin >> choice;
        if(cin.fail()) {
            cin.clear(); ignoreLine();
            cout << "Input error! Please enter a number.\n";
            continue;
        }
        ignoreLine();
        switch(choice) {
            case 1: lms.addStudent(); break;
            case 2: lms.viewStudents(); break;
            case 3: lms.searchStudent(); break;
            case 4: lms.updateStudent(); break;
            case 5: {
                int sid,cid; cout<<"Enter Student ID: "; cin>>sid; ignoreLine();
                cout<<"Enter Course ID: "; cin>>cid; ignoreLine();
                lms.enrollStudentInCourse(sid,cid); break;
            }
            case 6: {
                int sid; cout<<"Enter Student ID: "; cin>>sid; ignoreLine();
                lms.displayStudentGPA(sid); break;
            }
            case 7: lms.searchStudentPartial(); break;
            case 8: lms.bulkAddStudents(); break;
            case 0: break;
            default: cout<<"Invalid choice, please try again!\n"; break;
        }
    } while(choice != 0);
}
void teacherMenu(LMS &lms) {
    int choice;
    do {
        cout<<"\n--- TEACHER MENU ---\n";
        cout<<"1. Add Teacher\n2. View Teachers\n3. Update Teacher\n0. Back\nChoice: ";
        cin>>choice;
        if(cin.fail()) {
            cin.clear(); ignoreLine();
            cout << "Input error! Please enter a number.\n";
            continue;
        }
        ignoreLine();
        switch(choice) {
            case 1: lms.addTeacher(); break;
            case 2: lms.viewTeachers(); break;
            case 3: lms.updateTeacher(); break;
            case 0: break;
            default: cout<<"Invalid choice, please try again!\n"; break;
        }
    } while(choice != 0);
}
void courseMenu(LMS &lms) {
    int choice;
    do {
        cout<<"\n--- COURSE MENU ---\n";
        cout<<"1. Add Course\n2. View Courses\n3. Update Course\n4. Bulk Add Courses\n0. Back\nChoice: ";
        cin>>choice;
        if(cin.fail()) {
            cin.clear(); ignoreLine();
            cout << "Input error! Please enter a number.\n";
            continue;
        }
        ignoreLine();
        switch(choice) {
            case 1: lms.addCourse(); break;
            case 2: lms.viewCourses(); break;
            case 3: lms.updateCourse(); break;
            case 4: lms.bulkAddCourses(); break;
            case 0: break;
            default: cout<<"Invalid choice, please try again!\n"; break;
        }
    } while(choice != 0);
}
void assignmentMenu(LMS &lms) {
    int choice;
    do {
        cout<<"\n--- ASSIGNMENT MENU ---\n";
        cout<<"1. Add/Update Marks\n0. Back\nChoice: ";
        cin>>choice;
        if(cin.fail()) {
            cin.clear(); ignoreLine();
            cout << "Input error! Please enter a number.\n";
            continue;
        }
        ignoreLine();
        switch(choice) {
            case 1: lms.setStudentMarks(); break;
            case 0: break;
            default: cout<<"Invalid choice, please try again!\n"; break;
        }
    } while(choice != 0);
}
void rankingMenu(LMS &lms) {
    int choice;
    do {
        cout<<"\n--- RANKING MENU ---\n";
        cout<<"1. Rank Students in a Course\n2. Rank All Students by GPA\n0. Back\nChoice: ";
        cin>>choice;
        if(cin.fail()) {
            cin.clear(); ignoreLine();
            cout << "Input error! Please enter a number.\n";
            continue;
        }
        ignoreLine();
        switch(choice) {
            case 1: {
                int cid; cout<<"Enter Course ID: "; cin>>cid; ignoreLine();
                lms.rankStudentsInCourse(cid); break;
            }
            case 2: lms.rankAllStudentsByGPA(); break;
            case 0: break;
            default: cout<<"Invalid choice, please try again!\n"; break;
        }
    } while(choice != 0);
}

// ------------- Main -------------
int main() {
    LMS lms;
    int choice;
    do {
        cout << "\n==== LMS MAIN MENU ====\n";
        cout << "1. Student Management\n2. Teacher Management\n3. Course Management\n";
        cout << "4. Assignment & Marks\n5. Ranking & GPA\n0. Exit\nChoice: ";
        cin >> choice;
        if(cin.fail()) {
            cin.clear(); ignoreLine();
            cout << "Input error! Please enter a number.\n";
            continue;
        }
        ignoreLine();
        switch(choice) {
            case 1: studentMenu(lms); break;
            case 2: teacherMenu(lms); break;
            case 3: courseMenu(lms); break;
            case 4: assignmentMenu(lms); break;
            case 5: rankingMenu(lms); break;
            case 0: cout<<"Exiting LMS...\n"; break;
            default: cout<<"Invalid choice, please try again!\n"; break;
        }
    } while(choice != 0);
    return 0;
}

