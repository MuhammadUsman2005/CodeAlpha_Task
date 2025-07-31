#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

class Account;
class Transaction;

class Customer {
private:
    string customerId;
    string name;
    string address;
    string phoneNumber;
    vector<Account*> accounts;

public:
    Customer(const string& id, const string& name, 
             const string& address, const string& phone)
        : customerId(id), name(name), address(address), phoneNumber(phone) {}

    string getCustomerId() const { return customerId; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhoneNumber() const { return phoneNumber; }
    const vector<Account*>& getAccounts() const { return accounts; }

    void addAccount(Account* account);
    void displayCustomerInfo() const;
};

class Account {
private:
    string accountNumber;
    string accountType;
    double balance;
    Customer* owner;
    vector<Transaction*> transactions;

public:
    Account(const string& accNum, const string& accType, Customer* cust)
        : accountNumber(accNum), accountType(accType), balance(0.0), owner(cust) {}

    string getAccountNumber() const { return accountNumber; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    Customer* getOwner() const { return owner; }
    const vector<Transaction*>& getTransactions() const { return transactions; }

    void deposit(double amount, const string& description = "Deposit");
    bool withdraw(double amount, const string& description = "Withdrawal");
    bool transfer(Account& targetAccount, double amount, const string& description = "Transfer");
    void addTransaction(Transaction* transaction);
    void displayAccountInfo() const;
    void displayTransactionHistory(int limit = 10) const;
};

class Transaction {
private:
    string transactionId;
    string timestamp;
    string description;
    double amount;
    string type;
    Account* account;

public:
    Transaction(const string& transId, const string& desc, 
                double amt, const string& type, Account* acc)
        : transactionId(transId), description(desc), 
          amount(amt), type(type), account(acc) {
        
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp = timestamp.substr(0, timestamp.length()-1); // Remove newline
    }

    string getTransactionId() const { return transactionId; }
    string getTimestamp() const { return timestamp; }
    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    string getType() const { return type; }
    Account* getAccount() const { return account; }

    void displayTransactionInfo() const {
        cout << left << setw(20) << timestamp 
             << setw(15) << transactionId
             << setw(25) << description
             << setw(10) << type
             << fixed << setprecision(2) << "$" << amount << "\n";
    }
};

void Customer::addAccount(Account* account) {
    accounts.push_back(account);
}

void Customer::displayCustomerInfo() const {
    cout << "\nCustomer Information:\n";
    cout << "ID: " << customerId << "\n";
    cout << "Name: " << name << "\n";
    cout << "Address: " << address << "\n";
    cout << "Phone: " << phoneNumber << "\n";
    cout << "Number of Accounts: " << accounts.size() << "\n";
}

void Account::deposit(double amount, const string& description) {
    if (amount <= 0) {
        cout << "Invalid deposit amount.\n";
        return;
    }
    balance += amount;
    string transId = "T" + to_string(transactions.size() + 1);
    Transaction* transaction = new Transaction(transId, description, amount, "Credit", this);
    transactions.push_back(transaction);
    cout << "Deposit successful. New balance: $" << balance << "\n";
}

bool Account::withdraw(double amount, const string& description) {
    if (amount <= 0) {
        cout << "Invalid withdrawal amount.\n";
        return false;
    }
    if (balance < amount) {
        cout << "Insufficient funds.\n";
        return false;
    }
    balance -= amount;
    string transId = "T" + to_string(transactions.size() + 1);
    Transaction* transaction = new Transaction(transId, description, amount, "Debit", this);
    transactions.push_back(transaction);
    cout << "Withdrawal successful. New balance: $" << balance << "\n";
    return true;
}

bool Account::transfer(Account& targetAccount, double amount, const string& description) {
    if (this == &targetAccount) {
        cout << "Cannot transfer to the same account.\n";
        return false;
    }
    if (withdraw(amount, "Transfer to " + targetAccount.getAccountNumber())) {
        targetAccount.deposit(amount, "Transfer from " + accountNumber);
        return true;
    }
    return false;
}

void Account::addTransaction(Transaction* transaction) {
    transactions.push_back(transaction);
}

void Account::displayAccountInfo() const {
    cout << "\nAccount Information:\n";
    cout << "Account Number: " << accountNumber << "\n";
    cout << "Type: " << accountType << "\n";
    cout << "Balance: $" << fixed << setprecision(2) << balance << "\n";
    cout << "Owner: " << owner->getName() << "\n";
}

void Account::displayTransactionHistory(int limit) const {
    cout << "\nTransaction History (Last " << min(limit, (int)transactions.size()) << " transactions):\n";
    cout << left << setw(20) << "Timestamp"
         << setw(15) << "Transaction ID"
         << setw(25) << "Description"
         << setw(10) << "Type"
         << "Amount\n";
    
    int start = max(0, (int)transactions.size() - limit);
    for (int i = start; i < transactions.size(); i++) {
        transactions[i]->displayTransactionInfo();
    }
}

string generateCustomerId() {
    static int counter = 1000;
    return "CUST" + to_string(counter++);
}

string generateAccountNumber() {
    static int counter = 100000;
    return "ACCT" + to_string(counter++);
}

void displayMainMenu() {
    cout << "\n";
    cout << "=============================================\n";
    cout << "|           BANK MANAGEMENT SYSTEM          |\n";
    cout << "=============================================\n";
    cout << "| 1. Create New Customer                   |\n";
    cout << "| 2. Create New Account                    |\n";
    cout << "| 3. Deposit Money                         |\n";
    cout << "| 4. Withdraw Money                        |\n";
    cout << "| 5. Transfer Funds                        |\n";
    cout << "| 6. View Customer Details                 |\n";
    cout << "| 7. View Account Details                  |\n";
    cout << "| 8. View Transaction History              |\n";
    cout << "| 9. Exit                                  |\n";
    cout << "=============================================\n";
    cout << "| Enter your choice (1-9): ";
}

void displaySectionHeader(const string& title) {
    cout << "\n";
    cout << "=============================================\n";
    cout << "| " << left << setw(43) << title << " |\n";
    cout << "=============================================\n";
}

void displayHorizontalLine() {
    cout << "---------------------------------------------\n";
}

int main() {
    vector<Customer*> customers;
    vector<Account*> accounts;
    
    while (true) {
        displayMainMenu();
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            displaySectionHeader("INPUT ERROR");
            cout << "| Invalid input. Please enter a number (1-9) |\n";
            displayHorizontalLine();
            continue;
        }
        
        cin.ignore();
        
        switch (choice) {
            case 1: { 
                displaySectionHeader("CREATE NEW CUSTOMER");
                string name, address, phone;
                cout << "| Enter customer name: ";
                getline(cin, name);
                cout << "| Enter address: ";
                getline(cin, address);
                cout << "| Enter phone number: ";
                getline(cin, phone);
                
                string custId = generateCustomerId();
                Customer* newCustomer = new Customer(custId, name, address, phone);
                customers.push_back(newCustomer);
                
                displayHorizontalLine();
                cout << "| Customer created successfully!          |\n";
                cout << "| Customer ID: " << left << setw(30) << custId << " |\n";
                displayHorizontalLine();
                break;
            }
            
            case 2: {
                displaySectionHeader("CREATE NEW ACCOUNT");
                
                if (customers.empty()) {
                    cout << "| No customers available.                |\n";
                    cout << "| Please create a customer first.        |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select customer:                       |\n";
                displayHorizontalLine();
                for (int i = 0; i < customers.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (customers[i]->getName() + " (" + customers[i]->getCustomerId() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int custIndex;
                cout << "| Enter customer number: ";
                cin >> custIndex;
                custIndex--;
                
                if (custIndex < 0 || custIndex >= customers.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid customer selection!            |\n";
                    displayHorizontalLine();
                    break;
                }
                
                string accountType;
                cout << "| Enter account type (Savings/Checking): ";
                cin >> accountType;
                
                string accNum = generateAccountNumber();
                Account* newAccount = new Account(accNum, accountType, customers[custIndex]);
                customers[custIndex]->addAccount(newAccount);
                accounts.push_back(newAccount);
                
                displayHorizontalLine();
                cout << "| Account created successfully!          |\n";
                cout << "| Account Number: " << left << setw(27) << accNum << " |\n";
                displayHorizontalLine();
                break;
            }
            
            case 3: { 
                displaySectionHeader("DEPOSIT MONEY");
                
                if (accounts.empty()) {
                    cout << "| No accounts available!                 |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select account:                        |\n";
                displayHorizontalLine();
                for (int i = 0; i < accounts.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (accounts[i]->getAccountNumber() + " (" + accounts[i]->getOwner()->getName() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int accIndex;
                cout << "| Enter account number: ";
                cin >> accIndex;
                accIndex--;
                
                if (accIndex < 0 || accIndex >= accounts.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid account selection!             |\n";
                    displayHorizontalLine();
                    break;
                }
                
                double amount;
                cout << "| Enter amount to deposit: $";
                cin >> amount;
                
                accounts[accIndex]->deposit(amount);
                displayHorizontalLine();
                break;
            }
            
            case 4: { 
                displaySectionHeader("WITHDRAW MONEY");
                
                if (accounts.empty()) {
                    cout << "| No accounts available!                 |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select account:                        |\n";
                displayHorizontalLine();
                for (int i = 0; i < accounts.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (accounts[i]->getAccountNumber() + " (" + accounts[i]->getOwner()->getName() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int accIndex;
                cout << "| Enter account number: ";
                cin >> accIndex;
                accIndex--;
                
                if (accIndex < 0 || accIndex >= accounts.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid account selection!             |\n";
                    displayHorizontalLine();
                    break;
                }
                
                double amount;
                cout << "| Enter amount to withdraw: $";
                cin >> amount;
                
                if (accounts[accIndex]->withdraw(amount)) {
                    displayHorizontalLine();
                    cout << "| Withdrawal successful!                 |\n";
                }
                displayHorizontalLine();
                break;
            }
            
            case 5: { 
                displaySectionHeader("TRANSFER FUNDS");
                
                if (accounts.size() < 2) {
                    cout << "| Need at least 2 accounts to transfer!  |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select SOURCE account:                 |\n";
                displayHorizontalLine();
                for (int i = 0; i < accounts.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (accounts[i]->getAccountNumber() + " (" + accounts[i]->getOwner()->getName() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int srcIndex;
                cout << "| Enter source account number: ";
                cin >> srcIndex;
                srcIndex--;
                
                if (srcIndex < 0 || srcIndex >= accounts.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid source account!                |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select TARGET account:                 |\n";
                displayHorizontalLine();
                for (int i = 0; i < accounts.size(); i++) {
                    if (i != srcIndex) {
                        cout << "| " << i+1 << ". " << left << setw(38) 
                             << (accounts[i]->getAccountNumber() + " (" + accounts[i]->getOwner()->getName() + ")") 
                             << " |\n";
                    }
                }
                displayHorizontalLine();
                
                int targetIndex;
                cout << "| Enter target account number: ";
                cin >> targetIndex;
                targetIndex--;
                
                if (targetIndex < 0 || targetIndex >= accounts.size() || targetIndex == srcIndex) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid target account!                |\n";
                    displayHorizontalLine();
                    break;
                }
                
                double amount;
                cout << "| Enter amount to transfer: $";
                cin >> amount;
                
                if (accounts[srcIndex]->transfer(*accounts[targetIndex], amount)) {
                    displayHorizontalLine();
                    cout << "| Transfer successful!                   |\n";
                }
                displayHorizontalLine();
                break;
            }
            
            case 6: { 
                displaySectionHeader("CUSTOMER DETAILS");
                
                if (customers.empty()) {
                    cout << "| No customers available!                |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select customer:                       |\n";
                displayHorizontalLine();
                for (int i = 0; i < customers.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (customers[i]->getName() + " (" + customers[i]->getCustomerId() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int custIndex;
                cout << "| Enter customer number: ";
                cin >> custIndex;
                custIndex--;
                
                if (custIndex < 0 || custIndex >= customers.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid customer selection!            |\n";
                    displayHorizontalLine();
                    break;
                }
                
                displayHorizontalLine();
                customers[custIndex]->displayCustomerInfo();
                displayHorizontalLine();
                break;
            }
            
            case 7: { 
                displaySectionHeader("ACCOUNT DETAILS");
                
                if (accounts.empty()) {
                    cout << "| No accounts available!                 |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select account:                        |\n";
                displayHorizontalLine();
                for (int i = 0; i < accounts.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (accounts[i]->getAccountNumber() + " (" + accounts[i]->getOwner()->getName() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int accIndex;
                cout << "| Enter account number: ";
                cin >> accIndex;
                accIndex--;
                
                if (accIndex < 0 || accIndex >= accounts.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid account selection!             |\n";
                    displayHorizontalLine();
                    break;
                }
                
                displayHorizontalLine();
                accounts[accIndex]->displayAccountInfo();
                displayHorizontalLine();
                break;
            }
            
            case 8: {
                displaySectionHeader("TRANSACTION HISTORY");
                
                if (accounts.empty()) {
                    cout << "| No accounts available!                 |\n";
                    displayHorizontalLine();
                    break;
                }
                
                cout << "| Select account:                        |\n";
                displayHorizontalLine();
                for (int i = 0; i < accounts.size(); i++) {
                    cout << "| " << i+1 << ". " << left << setw(38) 
                         << (accounts[i]->getAccountNumber() + " (" + accounts[i]->getOwner()->getName() + ")") 
                         << " |\n";
                }
                displayHorizontalLine();
                
                int accIndex;
                cout << "| Enter account number: ";
                cin >> accIndex;
                accIndex--;
                
                if (accIndex < 0 || accIndex >= accounts.size()) {
                    displaySectionHeader("ERROR");
                    cout << "| Invalid account selection!             |\n";
                    displayHorizontalLine();
                    break;
                }
                
                int limit;
                cout << "| Enter number of transactions to show: ";
                cin >> limit;
                
                displayHorizontalLine();
                accounts[accIndex]->displayTransactionHistory(limit);
                displayHorizontalLine();
                break;
            }
            
            case 9:
                displaySectionHeader("EXIT SYSTEM");
                cout << "| Thank you for using our banking system! |\n";
                cout << "| Goodbye!                                |\n";
                displayHorizontalLine();
                
                for (auto& acc : accounts) {
                    for (auto& trans : acc->getTransactions()) {
                        delete trans;
                    }
                    delete acc;
                }
                
                for (auto& cust : customers) {
                    delete cust;
                }
                
                return 0;
                
            default:
                displaySectionHeader("INVALID CHOICE");
                cout << "| Please enter a number between 1-9!      |\n";
                displayHorizontalLine();
        }
    }
}
