#include <iostream>
#include <fstream>
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
    Friend() {
        surname = "";
        name = "";
        patronymic = "";
        birthdate = "";
        address = "";
        phone = "";
    }

    Friend(string s, string n, string p, string b, string a, string ph) {
        surname = s;
        name = n;
        patronymic = p;
        birthdate = b;
        address = a;
        phone = ph;
    }

    Friend(string s, string n, string b) {
        surname = s;
        name = n;
        patronymic = "";
        birthdate = b;
        address = "";
        phone = "";
    }

    void print() {
        cout << surname << " " << name << " " << patronymic << " " 
             << birthdate << " " << address << " " << phone << endl;
    }

    string getBirthdate() { return birthdate; }
    string getSurname() { return surname; }
    string getName() { return name; }
    string getPatronymic() { return patronymic; }
    string getAddress() { return address; }
    string getPhone() { return phone; }

    void setSurname(string s) { surname = s; }
    void setName(string n) { name = n; }
    void setPatronymic(string p) { patronymic = p; }
    void setBirthdate(string b) { birthdate = b; }
    void setAddress(string a) { address = a; }
    void setPhone(string ph) { phone = ph; }
};

class FriendsManager {
private:
    Friend friends[100];
    int friend_count;

public:
    FriendsManager() {
        friend_count = 0;
    }

    void print_friends() {
        for (int i = 0; i < friend_count; i++) {
            cout << "[" << i << "] ";
            friends[i].print();
        }
    }

    FriendsManager& operator+(const Friend& new_friend) {
        if (friend_count < 100) {
            friends[friend_count++] = new_friend;
        }
        return *this;
    }

    FriendsManager& operator-(int index) {
        if (index >= 0 && index < friend_count) {
            for (int i = index; i < friend_count - 1; i++) {
                friends[i] = friends[i + 1];
            }
            friend_count--;
        }
        return *this;
    }

    // Префиксный инкремент (++manager) - добавляет тестового друга
    FriendsManager& operator++() {
        if (friend_count < 100) {
            friends[friend_count++] = Friend("Иванов", "Иван", "Иванович", "12.12.2012", "Ленина", "12345678");
        }
        return *this;
    }

    // Префиксный декремент (--manager) - удаляет последнего друга
    FriendsManager& operator--() {
        if (friend_count > 0) {
            friend_count--;
        }
        return *this;
    }

    void add_friend() {
        string s, n, p, b, a, ph;
        cout << "Enter friend's surname: ";
        cin >> s;
        cout << "Enter friend's name: ";
        cin >> n;
        cout << "Enter friend's patronymic: ";
        cin >> p;
        cout << "Enter friend's birthdate: ";
        cin >> b;
        cout << "Enter friend's address: ";
        cin >> a;
        cout << "Enter friend's phone: ";
        cin >> ph;

        *this + Friend(s, n, p, b, a, ph);
    }

    void delete_friend() {
        int index;
        cout << "Enter index of friend to delete: ";
        cin >> index;
        *this - index;
    }

    void save_to_file() {
        ofstream f("friends.txt");
        for (int i = 0; i < friend_count; i++) {
            f << friends[i].getSurname() << " " 
              << friends[i].getName() << " "
              << friends[i].getPatronymic() << " "
              << friends[i].getBirthdate() << " "
              << friends[i].getAddress() << " "
              << friends[i].getPhone() << endl;
        }
        f.close();
    }

    void load_from_file() {
        ifstream f("friends.txt");
        friend_count = 0;
        
        string s, n, p, b, a, ph;
        while (f >> s >> n >> p >> b >> a >> ph) {
            friends[friend_count] = Friend(s, n, p, b, a, ph);
            friend_count++;
        }
        f.close();
    }

    void get_by_birthmonth() {
        int month;
        cout << "Enter birth month: ";
        cin >> month;
        
        for (int i = 0; i < friend_count; i++) {
            string bdate = friends[i].getBirthdate();
            int fmonth;
            sscanf(bdate.c_str(), "%*d.%d.%*d", &fmonth);
            
            if (fmonth == month) {
                friends[i].print();
            }
        }
    }
};

int main() {
    FriendsManager manager;
    manager.load_from_file();
    
    int choice;
    do {
        cout << "1. Print friends" << endl;
        cout << "2. Add friend (old method)" << endl;
        cout << "3. Delete friend (old method)" << endl;
        cout << "4. Get friends by birth month" << endl;
        cout << "5. Exit" << endl;
        cout << "6. Add friend (+ operator)" << endl;
        cout << "7. Delete friend (- operator)" << endl;
        cout << "8. Add test friend (++ operator)" << endl;
        cout << "9. Remove last friend (-- operator)" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                manager.print_friends();
                break;
            case 2:
                manager.add_friend();
                break;
            case 3:
                manager.delete_friend();
                break;
            case 4:
                manager.get_by_birthmonth();
                break;
            case 6: {
                string s, n, p, b, a, ph;
                cout << "Enter friend's surname: ";
                cin >> s;
                cout << "Enter friend's name: ";
                cin >> n;
                cout << "Enter friend's patronymic: ";
                cin >> p;
                cout << "Enter friend's birthdate: ";
                cin >> b;
                cout << "Enter friend's address: ";
                cin >> a;
                cout << "Enter friend's phone: ";
                cin >> ph;
                manager + Friend(s, n, p, b, a, ph);
                break;
            }
            case 7: {
                int index;
                cout << "Enter index of friend to delete: ";
                cin >> index;
                manager - index;
                break;
            }
            case 8:
                ++manager;
                break;
            case 9:
                --manager;
                break;
        }
    } while (choice != 5);
    
    manager.save_to_file();
    return 0;
}
