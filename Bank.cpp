#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Customer {
    int acc_no;
    char cust_nm[15];
    char mob[15];
    float bal;
};

struct Transaction {
    int acc_no;
    char trantype;
    float amt;
    char date[15];
};

int addCustomer() {
    fstream file;
    Customer cust;

    file.open("cust1", ios::out | ios::app | ios::binary);
    if (!file) {
        cout << "\nERROR opening cust1 file";
        return 0;
    }

    cout << "\n\nEnter Account number: ";
    cin >> cust.acc_no;
    cin.ignore();

    cout << "Enter customer name: ";
    cin.getline(cust.cust_nm, sizeof(cust.cust_nm));

    cout << "Enter mobile no.: ";
    cin.getline(cust.mob, sizeof(cust.mob));

    cout << "Enter Account Balance: ";
    cin >> cust.bal;

    file.write(reinterpret_cast<char*>(&cust), sizeof(Customer));
    file.close();
    return 1;
}

int deposit() {
    fstream file1, file2;
    Customer cust;
    Transaction tran;
    bool found = false;

    file1.open("cust1", ios::in | ios::out | ios::binary);
    if (!file1) {
        cout << "\nERROR opening cust1 file";
        return 0;
    }

    file2.open("trans", ios::out | ios::app | ios::binary);
    if (!file2) {
        cout << "\nERROR opening transaction file";
        file1.close();
        return 0;
    }

    cout << "\n\nEnter Account Number: ";
    cin >> tran.acc_no;

    while (file1.read(reinterpret_cast<char*>(&cust), sizeof(Customer))) {
        if (cust.acc_no == tran.acc_no) {
            found = true;
            break;
        }
    }

    if (found) {
        tran.trantype = 'd';
        cout << "Enter amount: ";
        cin >> tran.amt;
        cin.ignore();

        cout << "Enter deposit date: ";
        cin.getline(tran.date, sizeof(tran.date));

        cust.bal += tran.amt;

        file2.write(reinterpret_cast<char*>(&tran), sizeof(Transaction));

        file1.seekp(-static_cast<int>(sizeof(Customer)), ios::cur);
        file1.write(reinterpret_cast<char*>(&cust), sizeof(Customer));

    } else {
        cout << "\nThis account number does not exist.";
    }

    file1.close();
    file2.close();
    return 1;
}

int withdraw() {
    fstream file1, file2;
    Customer cust;
    Transaction tran;
    bool found = false;

    file1.open("cust1", ios::in | ios::out | ios::binary);
    if (!file1) {
        cout << "\nERROR opening cust1 file";
        return 0;
    }

    file2.open("trans", ios::out | ios::app | ios::binary);
    if (!file2) {
        cout << "\nERROR opening transaction file";
        file1.close();
        return 0;
    }

    cout << "\n\nEnter Account Number: ";
    cin >> tran.acc_no;

    while (file1.read(reinterpret_cast<char*>(&cust), sizeof(Customer))) {
        if (cust.acc_no == tran.acc_no) {
            found = true;
            break;
        }
    }

    if (found) {
        tran.trantype = 'w';
        cout << "Enter amount: ";
        cin >> tran.amt;
        cin.ignore();

        cout << "Enter withdrawal date: ";
        cin.getline(tran.date, sizeof(tran.date));

        cust.bal -= tran.amt;

        file2.write(reinterpret_cast<char*>(&tran), sizeof(Transaction));

        file1.seekp(-static_cast<int>(sizeof(Customer)), ios::cur);
        file1.write(reinterpret_cast<char*>(&cust), sizeof(Customer));

    } else {
        cout << "\nThis account number does not exist.";
    }

    file1.close();
    file2.close();
    return 1;
}

int displayAccounts() {
    ifstream file("cust1", ios::binary);
    Customer cust;

    if (!file) {
        cout << "\nERROR opening cust1 file";
        return 0;
    }

    cout << "\nReport on account balances:\n";
    while (file.read(reinterpret_cast<char*>(&cust), sizeof(Customer))) {
        cout << "\n" << cust.acc_no << "\t" << cust.cust_nm << "\t" << cust.mob << "\t" << cust.bal;
    }

    file.close();
    return 1;
}

int displayDeposits() {
    ifstream file("trans", ios::binary);
    Transaction tran;

    if (!file) {
        cout << "\nERROR opening trans file";
        return 0;
    }

    cout << "\nReport on deposits:\n";
    while (file.read(reinterpret_cast<char*>(&tran), sizeof(Transaction))) {
        if (tran.trantype == 'd') {
            cout << "\nAccount no.: " << tran.acc_no;
            cout << "\nStatus: " << tran.trantype;
            cout << "\nDeposit date: " << tran.date;
            cout << "\nDeposited amount: " << tran.amt << "\n";
        }
    }

    file.close();
    return 1;
}

int displayWithdrawals() {
    ifstream file("trans", ios::binary);
    Transaction tran;

    if (!file) {
        cout << "\nERROR opening trans file";
        return 0;
    }

    cout << "\nReport on withdrawals:\n";
    while (file.read(reinterpret_cast<char*>(&tran), sizeof(Transaction))) {
        if (tran.trantype == 'w') {
            cout << "\nAccount no.: " << tran.acc_no;
            cout << "\nStatus: " << tran.trantype;
            cout << "\nWithdrawal date: " << tran.date;
            cout << "\nWithdrawal amount: " << tran.amt << "\n";
        }
    }

    file.close();
    return 1;
}

int showBalance() {
    ifstream file("cust1", ios::binary);
    Customer cust;
    bool found = false;

    if (!file) {
        cout << "\nERROR opening cust1 file";
        return 0;
    }

    cout << "\n\nEnter Account Number: ";
    int accno;
    cin >> accno;

    while (file.read(reinterpret_cast<char*>(&cust), sizeof(Customer))) {
        if (cust.acc_no == accno) {
            cout << "\nAccount number: " << cust.acc_no;
            cout << "\nName: " << cust.cust_nm;
            cout << "\nMobile: " << cust.mob;
            cout << "\nBalance: " << cust.bal;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nAccount number does not exist.";
    }

    file.close();
    return 1;
}

int statement() {
    ifstream file1("cust1", ios::binary);
    Customer cust;
    Transaction tran;
    bool found = false;

    if (!file1) {
        cout << "\nERROR opening cust1 file";
        return 0;
    }

    cout << "\n\nEnter Account Number: ";
    int accno;
    cin >> accno;

    while (file1.read(reinterpret_cast<char*>(&cust), sizeof(Customer))) {
        if (cust.acc_no == accno) {
            cout << "\nAccount number: " << cust.acc_no;
            cout << "\nName: " << cust.cust_nm;
            cout << "\nMobile: " << cust.mob;
            cout << "\nBalance: " << cust.bal;
            found = true;
            break;
        }
    }

    file1.close();

    if (!found) {
        cout << "\nAccount number does not exist.";
        return 0;
    }

    ifstream file2("trans", ios::binary);
    if (!file2) {
        cout << "\nERROR opening trans file";
        return 0;
    }

    cout << "\n\nStatement:\n";
    cout << "DATE\t\tAMOUNT\t\tSTATUS\n";

    while (file2.read(reinterpret_cast<char*>(&tran), sizeof(Transaction))) {
        if (tran.acc_no == accno) {
            cout << tran.date << "\t" << tran.amt << "\t\t" << tran.trantype << "\n";
        }
    }

    file2.close();
    return 1;
}

int main() {
    int choice = 1;

    while (choice != 0) {
        cout << "\nSelect choice from menu:\n\n";
        cout << "1. Add new customer details\n";
        cout << "2. Deposits\n";
        cout << "3. Withdrawals\n";
        cout << "4. Display all accounts\n";
        cout << "5. Display all deposits\n";
        cout << "6. Display all withdrawals\n";
        cout << "7. Show balance\n";
        cout << "8. Show Statements\n";
        cout << "0. EXIT\n\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: displayAccounts(); break;
            case 5: displayDeposits(); break;
            case 6: displayWithdrawals(); break;
            case 7: showBalance(); break;
            case 8: statement(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
    return 0;
}
