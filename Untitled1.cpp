#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
    int accountNo;
    string name;
    char type; // S = Savings, C = Current
    double balance;

public:
    Account() {}
    void createAccount();
    void showAccount() const;
    void modify();
    void deposit(double amount);
    void withdraw(double amount);
    void report() const;
    int getAccountNo() const;
    double getBalance() const;
    char getType() const;
};

void Account::createAccount() {
    cout << "\nEnter Account No: ";
    cin >> accountNo;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Type of Account (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "Enter Initial Deposit: ";
    cin >> balance;
    cout << "\nAccount Created Successfully!\n";
}

void Account::showAccount() const {
    cout << "\nAccount No: " << accountNo;
    cout << "\nAccount Holder: " << name;
    cout << "\nType of Account: " << type;
    cout << "\nBalance: ?" << balance << endl;
}

void Account::modify() {
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Modify Type of Account (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "Modify Balance: ";
    cin >> balance;
}

void Account::deposit(double amount) { balance += amount; }

void Account::withdraw(double amount) {
    if (balance - amount < 0)
        cout << "Insufficient Balance!\n";
    else
        balance -= amount;
}

void Account::report() const {
    cout << setw(10) << accountNo << setw(20) << name << setw(10) << type
         << setw(12) << balance << endl;
}

int Account::getAccountNo() const { return accountNo; }
double Account::getBalance() const { return balance; }
char Account::getType() const { return type; }

// ================= FILE HANDLING FUNCTIONS =================

void writeAccount();
void displayAll();
void displaySp(int n);
void modifyAccount(int n);
void deleteAccount(int n);
void depositWithdraw(int n, int option);

// ================= MAIN MENU =================
int main() {
    char choice;
    int num;

    do {
        cout << "\n\n\t*** BANK MANAGEMENT SYSTEM ***";
        cout << "\n\n\t1. NEW ACCOUNT";
        cout << "\n\t2. DEPOSIT AMOUNT";
        cout << "\n\t3. WITHDRAW AMOUNT";
        cout << "\n\t4. BALANCE ENQUIRY";
        cout << "\n\t5. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t6. CLOSE AN ACCOUNT";
        cout << "\n\t7. MODIFY AN ACCOUNT";
        cout << "\n\t8. EXIT";
        cout << "\n\n\tSelect Option (1-8): ";
        cin >> choice;

        switch (choice) {
        case '1':
            writeAccount();
            break;
        case '2':
            cout << "\nEnter Account No: ";
            cin >> num;
            depositWithdraw(num, 1);
            break;
        case '3':
            cout << "\nEnter Account No: ";
            cin >> num;
            depositWithdraw(num, 2);
            break;
        case '4':
            cout << "\nEnter Account No: ";
            cin >> num;
            displaySp(num);
            break;
        case '5':
            displayAll();
            break;
        case '6':
            cout << "\nEnter Account No: ";
            cin >> num;
            deleteAccount(num);
            break;
        case '7':
            cout << "\nEnter Account No: ";
            cin >> num;
            modifyAccount(num);
            break;
        case '8':
            cout << "\nThank you for using our system!\n";
            break;
        default:
            cout << "\nInvalid Option!\n";
        }
        cin.ignore();
        cin.get();
    } while (choice != '8');

    return 0;
}

// ================= FILE FUNCTIONS =================

void writeAccount() {
    Account ac;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void displayAll() {
    Account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be open!\n";
        return;
    }
    cout << "\n\n\tACCOUNT HOLDER LIST\n\n";
    cout << setw(10) << "A/c No" << setw(20) << "Name" << setw(10)
         << "Type" << setw(12) << "Balance\n";
    cout << "--------------------------------------------------------\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.report();
    }
    inFile.close();
}

void displaySp(int n) {
    Account ac;
    bool found = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be open!\n";
        return;
    }
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNo() == n) {
            ac.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\nAccount number does not exist.\n";
}

void modifyAccount(int n) {
    Account ac;
    bool found = false;
    fstream file;
    file.open("account.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be open!\n";
        return;
    }
    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNo() == n) {
            ac.showAccount();
            cout << "\nEnter New Account Details:\n";
            ac.modify();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nAccount Updated Successfully.\n";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\nAccount number not found.\n";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be open!\n";
        return;
    }
    outFile.open("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNo() != n) {
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("temp.dat", "account.dat");
    cout << "\nAccount Deleted Successfully.\n";
}

void depositWithdraw(int n, int option) {
    Account ac;
    fstream file;
    bool found = false;
    file.open("account.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be open!\n";
        return;
    }
    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNo() == n) {
            ac.showAccount();
            double amt;
            if (option == 1) {
                cout << "\nEnter amount to deposit: ";
                cin >> amt;
                ac.deposit(amt);
            } else if (option == 2) {
                cout << "\nEnter amount to withdraw: ";
                cin >> amt;
                ac.withdraw(amt);
            }
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nRecord Updated Successfully.\n";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\nAccount number not found.\n";
}
