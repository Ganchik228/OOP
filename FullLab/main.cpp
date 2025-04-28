#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <memory>

using namespace std;

class Person {
protected:
    string firstName;
    string lastName;
    tm birthDate;
    
public:
    Person(string first, string last, int day, int month, int year) 
        : firstName(first), lastName(last) {
        birthDate.tm_mday = day;
        birthDate.tm_mon = month - 1;
        birthDate.tm_year = year - 1900;
    }

    virtual ~Person() = default;
    
    virtual void printInfo() const = 0;
    
    string getFullName() const {
        return firstName + " " + lastName;
    }
    
    tm getBirthDate() const { return birthDate; }
    
    void setBirthDate(int day, int month, int year) {
        birthDate.tm_mday = day;
        birthDate.tm_mon = month - 1;
        birthDate.tm_year = year - 1900;
    }
};

class Friend : public Person {
private:
    string middleName;
    string address;
    string phone;

public:
    Friend(string first = "", string last = "", string middle = "", 
           int day = 1, int month = 1, int year = 1900,
           string addr = "", string ph = "")
        : Person(first, last, day, month, year), middleName(middle),
          address(addr), phone(ph) {}

    void printInfo() const override {
        cout << "Friend: " << firstName << " " << lastName << " " << middleName << endl;
        cout << "Birth date: " << birthDate.tm_mday << "." 
             << (birthDate.tm_mon + 1) << "." << (birthDate.tm_year + 1900) << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl << endl;
    }

    friend class FriendsManager;
};

class Colleague : public Person {
protected:
    string position;
    string company;

public:
    Colleague(string first, string last, int day, int month, int year,
              string pos, string comp)
        : Person(first, last, day, month, year), position(pos), company(comp) {}

    void printInfo() const override {
        cout << "Colleague: " << firstName << " " << lastName << endl;
        cout << "Position: " << position << " at " << company << endl;
        cout << "Birth date: " << birthDate.tm_mday << "." 
             << (birthDate.tm_mon + 1) << "." << (birthDate.tm_year + 1900) << endl << endl;
    }

    string getPosition() const { return position; }
    string getCompany() const { return company; }
};

class WorkFriend : public Colleague {
private:
    string commonProjects;

public:
    WorkFriend(string first, string last, int day, int month, int year,
               string pos, string comp, string projects)
        : Colleague(first, last, day, month, year, pos, comp), commonProjects(projects) {}

    void printInfo() const override {
        cout << "Work Friend: " << firstName << " " << lastName << endl;
        cout << "Position: " << position << " at " << company << endl;
        cout << "Common Projects: " << commonProjects << endl;
        cout << "Birth date: " << birthDate.tm_mday << "." 
             << (birthDate.tm_mon + 1) << "." << (birthDate.tm_year + 1900) << endl << endl;
    }

    string getCommonProjects() const { return commonProjects; }
};

class FriendsManager {
private:
    vector<shared_ptr<Person>> people;

    friend void printFriendCount(const FriendsManager& fm);

public:
    void addPerson(shared_ptr<Person> person) {
        people.push_back(person);
    }

    void printAll() const {
        for (const auto& p : people) {
            p->printInfo();
        }
    }

    vector<shared_ptr<Person>> findByBirthMonth(int month) const {
        vector<shared_ptr<Person>> result;
        for (const auto& p : people) {
            if (p->getBirthDate().tm_mon + 1 == month) {
                result.push_back(p);
            }
        }
        return result;
    }

    void deletePerson(int index) {
        if (index >= 0 && index < people.size()) {
            people.erase(people.begin() + index);
        }
    }

    void increaseAllAges() {
        for (auto& p : people) {
            tm bd = p->getBirthDate();
            p->setBirthDate(bd.tm_mday, bd.tm_mon + 1, bd.tm_year + 1900 - 1);
        }
    }

    void decreaseAllAges() {
        for (auto& p : people) {
            tm bd = p->getBirthDate();
            p->setBirthDate(bd.tm_mday, bd.tm_mon + 1, bd.tm_year + 1900 + 1);
        }
    }

    size_t count() const {
        return people.size();
    }

    void saveToFile(const string& filename = "friends.dat") {
        ofstream out(filename);
        if (!out) {
            throw runtime_error("Cannot open file for writing");
        }

        for (const auto& p : people) {
            if (auto f = dynamic_cast<Friend*>(p.get())) {
                out << "FRIEND\n";
                out << f->getFullName() << "\n";
                out << f->getBirthDate().tm_mday << " " 
                    << f->getBirthDate().tm_mon + 1 << " " 
                    << f->getBirthDate().tm_year + 1900 << "\n";
                out << f->address << "\n";
                out << f->phone << "\n";
            }
            else if (auto wf = dynamic_cast<WorkFriend*>(p.get())) {
                out << "WORKFRIEND\n";
                out << wf->getFullName() << "\n";
                out << wf->getBirthDate().tm_mday << " " 
                    << wf->getBirthDate().tm_mon + 1 << " " 
                    << wf->getBirthDate().tm_year + 1900 << "\n";
                out << wf->getPosition() << "\n";
                out << wf->getCompany() << "\n";
                out << wf->getCommonProjects() << "\n";
            }
            else if (auto c = dynamic_cast<Colleague*>(p.get())) {
                out << "COLLEAGUE\n";
                out << c->getFullName() << "\n";
                out << c->getBirthDate().tm_mday << " " 
                    << c->getBirthDate().tm_mon + 1 << " " 
                    << c->getBirthDate().tm_year + 1900 << "\n";
                out << c->getPosition() << "\n";
                out << c->getCompany() << "\n";
            }
            out << "---\n";
        }
    }

    void loadFromFile(const string& filename = "friends.dat") {
        ifstream in(filename);
        if (!in) {
            throw runtime_error("Cannot open file for reading");
        }

        people.clear();
        string line;
        while (getline(in, line)) {
            if (line == "FRIEND") {
                string firstName, lastName;
                getline(in, firstName);
                getline(in, lastName);
                
                int day, month, year;
                in >> day >> month >> year;
                in.ignore();
                
                string address, phone;
                getline(in, address);
                getline(in, phone);
                
                people.push_back(make_shared<Friend>(
                    firstName, lastName, "", day, month, year, address, phone));
            }
            else if (line == "WORKFRIEND") {
                string firstName, lastName;
                getline(in, firstName);
                getline(in, lastName);
                
                int day, month, year;
                in >> day >> month >> year;
                in.ignore();
                
                string position, company, projects;
                getline(in, position);
                getline(in, company);
                getline(in, projects);
                
                people.push_back(make_shared<WorkFriend>(
                    firstName, lastName, day, month, year, position, company, projects));
            }
            else if (line == "COLLEAGUE") {
                string firstName, lastName;
                getline(in, firstName);
                getline(in, lastName);
                
                int day, month, year;
                in >> day >> month >> year;
                in.ignore();
                
                string position, company;
                getline(in, position);
                getline(in, company);
                
                people.push_back(make_shared<Colleague>(
                    firstName, lastName, day, month, year, position, company));
            }
            getline(in, line);
        }
    }
};

void printFriendCount(const FriendsManager& fm) {
    int friendCount = 0;
    for (const auto& p : fm.people) {
        if (dynamic_cast<Friend*>(p.get())) {
            friendCount++;
        }
    }
    cout << "Total friends: " << friendCount << endl;
}

void printMenu() {
    cout << "1. Add person" << endl;
    cout << "2. Find by birth month" << endl;
    cout << "3. Show all" << endl;
    cout << "4. Delete person" << endl;
    cout << "5. Increase all ages by 1 year" << endl;
    cout << "6. Decrease all ages by 1 year" << endl;
    cout << "7. Print friend count" << endl;
    cout << "8. Exit" << endl;
    cout << "Select action: ";
}

shared_ptr<Person> inputPerson() {
    cout << "Select person type:\n";
    cout << "1. Friend\n2. Colleague\n3. Work Friend\n";
    int type;
    cin >> type;
    cin.ignore();

    string first, last, middle, addr, phone, pos, comp, projects;
    int day, month, year;

    cout << "Enter first name: ";
    getline(cin, first);
    cout << "Enter last name: ";
    getline(cin, last);
    cout << "Enter birth date (day month year): ";
    cin >> day >> month >> year;
    cin.ignore();

    switch(type) {
        case 1: {
            cout << "Enter middle name: ";
            getline(cin, middle);
            cout << "Enter address: ";
            getline(cin, addr);
            cout << "Enter phone: ";
            getline(cin, phone);
            return make_shared<Friend>(first, last, middle, day, month, year, addr, phone);
        }
        case 2: {
            cout << "Enter position: ";
            getline(cin, pos);
            cout << "Enter company: ";
            getline(cin, comp);
            return make_shared<Colleague>(first, last, day, month, year, pos, comp);
        }
        case 3: {
            cout << "Enter position: ";
            getline(cin, pos);
            cout << "Enter company: ";
            getline(cin, comp);
            cout << "Enter common projects: ";
            getline(cin, projects);
            return make_shared<WorkFriend>(first, last, day, month, year, pos, comp, projects);
        }
        default:
            throw invalid_argument("Invalid person type");
    }
}

int main() {
    FriendsManager manager;
    manager.loadFromFile();
    
    int choice;
    do {
        printMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                try {
                    auto person = inputPerson();
                    manager.addPerson(person);
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter month (1-12): ";
                int month;
                cin >> month;
                auto found = manager.findByBirthMonth(month);
                if (found.empty()) {
                    cout << "No persons born in this month." << endl;
                } else {
                    for (const auto& p : found) {
                        p->printInfo();
                    }
                }
                break;
            }
            case 3:
                manager.printAll();
                break;
            case 4: {
                manager.printAll();
                if (manager.count() > 0) {
                    cout << "Enter person number to delete (1-" << manager.count() << "): ";
                    int index;
                    cin >> index;
                    manager.deletePerson(index - 1);
                }
                break;
            }
            case 5: {
                manager.increaseAllAges();
                cout << "All ages increased by 1 year." << endl;
                break;
            }
            case 6: {
                manager.decreaseAllAges();
                cout << "All ages decreased by 1 year." << endl;
                break;
            }
            case 7:
                printFriendCount(manager);
                break;
            case 8:
                manager.saveToFile();
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 8);
    
    return 0;
}
