#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Student {
    int id;
    string name;
    double marks;
};

bool writeCsv(const string& file, const vector<Student>& list) {
    ofstream out(file);
    if (!out) return false;
    out << "id,name,marks\n";
    for (const auto& s : list) out << s.id << "," << s.name << "," << s.marks << "\n";
    out.flush();
    return static_cast<bool>(out);
}

bool readCsv(const string& file, vector<Student>& list) {
    ifstream in(file);
    if (!in) return false;
    string line;
    getline(in, line);
    int lineNo = 1;
    while (getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, marks;
        if (!getline(ss, id, ',') || !getline(ss, name, ',') || !getline(ss, marks, ',')) {
            cout << "  Skipping malformed line " << lineNo << ": " << line << endl;
            continue;
        }
        try {
            list.push_back({stoi(id), name, stod(marks)});
        } catch (const exception&) {
            cout << "  Skipping line " << lineNo << " (invalid number): " << line << endl;
        }
    }
    return true;
}

void showStudents(const vector<Student>& list) {
    cout << "ID  Name     Marks" << endl;
    for (const auto& s : list) cout << s.id << "   " << s.name << "   " << s.marks << endl;
}

int main() {
    cout << "Name: Nahushraj" << endl;

    vector<Student> students = {{1, "Nahushraj", 88.5}, {2, "Jehal", 91.0}, {3, "Aarav", 76.25}};
    if (!writeCsv("students.csv", students)) {
        cerr << "Cannot write students.csv" << endl;
        return 1;
    }
    cout << "Wrote " << students.size() << " records to students.csv" << endl;

    cout << "\n--- Reading students.csv ---" << endl;
    vector<Student> loaded;
    if (!readCsv("students.csv", loaded)) { cerr << "Cannot open students.csv" << endl; return 1; }
    showStudents(loaded);

    cout << "\n--- Error handling: missing file ---" << endl;
    vector<Student> none;
    if (!readCsv("does_not_exist.csv", none)) cout << "  Could not open does_not_exist.csv (handled safely)" << endl;

    cout << "\n--- Error handling: malformed lines ---" << endl;
    {
        ofstream bad("malformed.csv");
        bad << "id,name,marks\n4,Riya,67.5\nthis line is wrong\n5,Karan,abc\n6,Neha,80\n";
    }
    vector<Student> partial;
    readCsv("malformed.csv", partial);
    cout << "  Valid records loaded: " << partial.size() << endl;

    cout << "\n--- File position functions ---" << endl;
    { ofstream f("pos.txt"); f << "ABCDEFGHIJ"; }
    fstream f("pos.txt", ios::in | ios::out);
    cout << "Initial tellg() = " << f.tellg() << ", tellp() = " << f.tellp() << endl;
    f.seekg(4);
    char ch;
    f.get(ch);
    cout << "seekg(4) then get() -> '" << ch << "', tellg() = " << f.tellg() << endl;
    f.seekp(2);
    f.put('x');
    f.flush();
    cout << "seekp(2) then put('x'), tellp() = " << f.tellp() << endl;
    f.seekg(0);
    string content;
    getline(f, content);
    cout << "File content now: " << content << endl;
    f.seekg(0, ios::end);
    cout << "File size via seekg(0, end)/tellg() = " << f.tellg() << " bytes" << endl;
    return 0;
}