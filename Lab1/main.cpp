#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Friend {
   string surname;
   string name;
   string patronymic;
   string birthdate;
   string address;
   string phone;
};

vector<Friend> friends;
int friend_count = 0;

void print_friends() {
   for (int i = 0; i < friend_count; i++) {
      cout << friends[i].surname << " " << friends[i].name << " " 
           << friends[i].patronymic << " " << friends[i].birthdate << " " 
           << friends[i].address << " " << friends[i].phone << endl;
   }
}

void add_friend() {
   Friend f;
   cout << "Enter friend's surname: ";
   cin >> f.surname;
   cout << "Enter friend's name: ";
   cin >> f.name;
   cout << "Enter friend's patronymic: ";
   cin >> f.patronymic;
   cout << "Enter friend's birthdate: ";
   cin >> f.birthdate;
   cout << "Enter friend's address: ";
   cin >> f.address;
   cout << "Enter friend's phone: ";
   cin >> f.phone;
   friends.push_back(f);
   friend_count++;
}

void delete_friend() {
   int index;
   cout << "Enter index of friend to delete: ";
   cin >> index;
   if (index < 0 || index >= friend_count) {
      cout << "Invalid index" << endl;
      return;
   }
   friends.erase(friends.begin() + index);
   friend_count--;
}

void save_friends_to_file() {
   ofstream fout("friends.txt");
   for (int i = 0; i < friend_count; i++) {
      fout << friends[i].surname << " " << friends[i].name << " " 
           << friends[i].patronymic << " " << friends[i].birthdate << " " 
           << friends[i].address << " " << friends[i].phone << endl;
   }
}

void load_friends_from_file() {
   ifstream fin("friends.txt");
   friends.clear();
   friend_count = 0;
   Friend f;
   while (fin >> f.surname >> f.name >> f.patronymic 
              >> f.birthdate >> f.address >> f.phone) {
      friends.push_back(f);
      friend_count++;
   }
}

void get_friends_by_birthmonth() {
   int month;
   cout << "Enter birth month: ";
   cin >> month;
   for (int i = 0; i < friend_count; i++) {
      int fmonth;
      sscanf(friends[i].birthdate.c_str(), "%*d.%d.%*d", &fmonth);
      if (fmonth == month) {
         cout << friends[i].surname << " " << friends[i].name << " " 
              << friends[i].patronymic << " " << friends[i].birthdate << " " 
              << friends[i].address << " " << friends[i].phone << endl;
      }
   }
}

int main() {
   load_friends_from_file();
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
            print_friends();
            break;
         case 2:
            add_friend();
            break;
         case 3:
            delete_friend();
            break;
         case 4:
            get_friends_by_birthmonth();
            break;
      }
   } while (choice != 5);
   save_friends_to_file();
   return 0;
}
