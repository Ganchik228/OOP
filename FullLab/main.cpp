#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Friend {
private:
    string surname;
    string name;
    string patronymic;
    string birthdate;
    string address;
    string phone;

public:
    Friend() : surname("Иванов"), name("Иван"), patronymic("Иванович"),
               birthdate("01.01.2000"), address("ул. Ленина, 1"), phone("+71234567890") {}

    Friend(const string& sname, const string& nname) : surname(sname), name(nname),
        patronymic(""), birthdate(""), address(""), phone("") {}

    Friend(const string& sname, const string& nname, const string& patr,
           const string& bdate, const string& addr, const string& ph) :
        surname(sname), name(nname), patronymic(patr),
        birthdate(bdate), address(addr), phone(ph) {}

    void print() const {
        cout << surname << " " << name << " " << patronymic << " "
             << birthdate << " " << address << " " << phone << endl;
    }

    string getSurname() const { return surname; }
    string getName() const { return name; }
    string getPatronymic() const { return patronymic; }
    string getBirthdate() const { return birthdate; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
};


class App {
private:
    vector<Friend> friends;

public:
    void printFriends() const {
        for (const auto& f : friends) {
            f.print();
        }
    }

    void addFriend(const Friend& f) {
        friends.push_back(f);
    }

    void deleteFriend(int index) {
        if (index < 0 || index >= friends.size()) {
            cout << "Неверный индекс" << endl;
            return;
        }
        friends.erase(friends.begin() + index);
    }

    void saveToFile(const string& filename) const {
        ofstream fout(filename);
        for (const auto& f : friends) {
            fout << f.getSurname() << " " << f.getName() << " " 
                 << f.getPatronymic() << " " << f.getBirthdate() << " "
                 << f.getAddress() << " " << f.getPhone() << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream fin(filename);
        friends.clear();
        string sname, name, patr, bdate, addr, ph;
        while (fin >> sname >> name >> patr >> bdate >> addr >> ph) {
            friends.emplace_back(sname, name, patr, bdate, addr, ph);
        }
    }

    void getFriendsByBirthmonth(int month) const {
        for (const auto& f : friends) {
            int fmonth;
            sscanf(f.getBirthdate().c_str(), "%*d.%d.%*d", &fmonth);
            if (fmonth == month) {
                f.print();
            }
        }
    }
};


int main() {
    App app;
    app.loadFromFile("friends.txt");

    int choice;
    do {
        cout << "1. Print friends" << endl;
        cout << "2. Add friend" << endl;
        cout << "3. Delete friend" << endl;
        cout << "4. Get friends by birth month" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                app.printFriends();
                break;
            case 2: {
                string sname, name, patr, bdate, addr, ph;
                cout << "Enter friend's surname: ";
                cin >> sname;
                cout << "Enter friend's name: ";
                cin >> name;
                cout << "Enter friend's patronymic: ";
                cin >> patr;
                cout << "Enter friend's birthdate: ";
                cin >> bdate;
                cout << "Enter friend's address: ";
                cin.ignore();
                getline(cin, addr);
                cout << "Enter friend's phone: ";
                cin >> ph;
                app.addFriend(Friend(sname, name, patr, bdate, addr, ph));
                break;
            }
            case 3: {
                int index;
                cout << "Enter index of friend to delete: ";
                cin >> index;
                app.deleteFriend(index);
                break;
            }
            case 4: {
                int month;
                cout << "Enter birth month: ";
                cin >> month;
                app.getFriendsByBirthmonth(month);
                break;
            }
        }
    } while (choice != 5);

    app.saveToFile("friends.txt");
    return 0;
}
