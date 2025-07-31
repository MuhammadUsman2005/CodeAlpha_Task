#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <map>

using namespace std;

class UserAuthenticationSystem {
private:
    const string DATABASE_FILE = "users_database.txt";
    const string DELIMITER = "|";
    
    string hashPassword(const string& plainPassword) {
        string hashedPassword = "";
        for (char character : plainPassword) {
            hashedPassword += to_string((int)character + 7); // Simple Caesar cipher
            hashedPassword += "#";
        }
        return hashedPassword;
    }
    
    bool isValidUsername(const string& username) {
        if (username.length() < 3 || username.length() > 20) {
            cout << "Error: Username must be between 3 and 20 characters.\n";
            return false;
        }
        
        for (char character : username) {
            if (!isalnum(character) && character != '_') {
                cout << "Error: Username can only contain letters, numbers, and underscores.\n";
                return false;
            }
        }
        
        if (!isalpha(username[0])) {
            cout << "Error: Username must start with a letter.\n";
            return false;
        }
        
        return true;
    }
    
    bool isValidPassword(const string& password) {
        if (password.length() < 6 || password.length() > 50) {
            cout << "Error: Password must be between 6 and 50 characters.\n";
            return false;
        }
        
        bool hasUpperCase = false;
        bool hasLowerCase = false;
        bool hasDigit = false;
        bool hasSpecialChar = false;
        
        for (char character : password) {
            if (isupper(character)) hasUpperCase = true;
            else if (islower(character)) hasLowerCase = true;
            else if (isdigit(character)) hasDigit = true;
            else if (!isalnum(character)) hasSpecialChar = true;
        }
        
        if (!hasUpperCase) {
            cout << "Error: Password must contain at least one uppercase letter.\n";
            return false;
        }
        
        if (!hasLowerCase) {
            cout << "Error: Password must contain at least one lowercase letter.\n";
            return false;
        }
        
        if (!hasDigit) {
            cout << "Error: Password must contain at least one digit.\n";
            return false;
        }
        
        if (!hasSpecialChar) {
            cout << "Error: Password must contain at least one special character.\n";
            return false;
        }
        
        return true;
    }
    
    bool isUsernameExists(const string& username) {
        ifstream databaseFile(DATABASE_FILE);
        string currentLine;
        
        if (!databaseFile.is_open()) {
            return false; 
        }
        
        while (getline(databaseFile, currentLine)) {
            if (currentLine.empty()) continue;
            
            size_t delimiterPosition = currentLine.find(DELIMITER);
            if (delimiterPosition != string::npos) {
                string existingUsername = currentLine.substr(0, delimiterPosition);
                if (existingUsername == username) {
                    databaseFile.close();
                    return true;
                }
            }
        }
        
        databaseFile.close();
        return false;
    }
    
    map<string, string> loadUsersFromDatabase() {
        map<string, string> usersDatabase;
        ifstream databaseFile(DATABASE_FILE);
        string currentLine;
        
        if (!databaseFile.is_open()) {
            return usersDatabase; // Return empty map if file doesn't exist
        }
        
        while (getline(databaseFile, currentLine)) {
            if (currentLine.empty()) continue;
            
            size_t delimiterPosition = currentLine.find(DELIMITER);
            if (delimiterPosition != string::npos) {
                string storedUsername = currentLine.substr(0, delimiterPosition);
                string storedHashedPassword = currentLine.substr(delimiterPosition + 1);
                usersDatabase[storedUsername] = storedHashedPassword;
            }
        }
        
        databaseFile.close();
        return usersDatabase;
    }
    
    bool saveUserToDatabase(const string& username, const string& hashedPassword) {
        ofstream databaseFile(DATABASE_FILE, ios::app);
        
        if (!databaseFile.is_open()) {
            cout << "Error: Unable to access user database.\n";
            return false;
        }
        
        databaseFile << username << DELIMITER << hashedPassword << "\n";
        databaseFile.close();
        return true;
    }
    
public:
   
    bool registerUser() {
        string inputUsername, inputPassword, confirmPassword;
        
        cout << "\n" << string(50, '=') << "\n";
        cout << "              USER REGISTRATION\n";
        cout << string(50, '=') << "\n\n";
        
        do {
            cout << "Enter username (3-20 chars, start with letter, alphanumeric + underscore): ";
            getline(cin, inputUsername);
            
            if (inputUsername.empty()) {
                cout << "Error: Username cannot be empty.\n";
                continue;
            }
            
        } while (!isValidUsername(inputUsername));
        
        // Here we check for duplicate username
        if (isUsernameExists(inputUsername)) {
            cout << "Error: Username '" << inputUsername << "' already exists. Please choose a different username.\n";
            return false;
        }
        
        // 
        do {
            cout << "Enter password (6-50 chars, must include: uppercase, lowercase, digit, special char): ";
            getline(cin, inputPassword);
            
            if (inputPassword.empty()) {
                cout << "Error: Password cannot be empty.\n";
                continue;
            }
            
        } while (!isValidPassword(inputPassword));
        
        cout << "Confirm password: ";
        getline(cin, confirmPassword);
        
        if (inputPassword != confirmPassword) {
            cout << "Error: Passwords do not match.\n";
            return false;
        }
        
        string hashedPassword = hashPassword(inputPassword);
        
        if (saveUserToDatabase(inputUsername, hashedPassword)) {
            cout << "\nSuccess: User '" << inputUsername << "' registered successfully!\n";
            cout << "You can now log in with your credentials.\n";
            return true;
        } else {
            cout << "Error: Registration failed. Please try again.\n";
            return false;
        }
    }
    
    bool loginUser() {
        string inputUsername, inputPassword;
        
        cout << "\n" << string(50, '=') << "\n";
        cout << "                USER LOGIN\n";
        cout << string(50, '=') << "\n\n";
        
        cout << "Enter username: ";
        getline(cin, inputUsername);
        
        if (inputUsername.empty()) {
            cout << "Error: Username cannot be empty.\n";
            return false;
        }
        
        cout << "Enter password: ";
        getline(cin, inputPassword);
        
        if (inputPassword.empty()) {
            cout << "Error: Password cannot be empty.\n";
            return false;
        }
        
        map<string, string> usersDatabase = loadUsersFromDatabase();
        
        if (usersDatabase.empty()) {
            cout << "Error: No users found. Please register first.\n";
            return false;
        }
        
        if (usersDatabase.find(inputUsername) == usersDatabase.end()) {
            cout << "Error: Username '" << inputUsername << "' not found.\n";
            return false;
        }
        
        // Here we verify password
        string hashedInputPassword = hashPassword(inputPassword);
        string storedHashedPassword = usersDatabase[inputUsername];
        
        if (hashedInputPassword == storedHashedPassword) {
            cout << "\nSuccess: Login successful! Welcome, " << inputUsername << "!\n";
            cout << "You are now logged into the system.\n";
            return true;
        } else {
            cout << "Error: Incorrect password for username '" << inputUsername << "'.\n";
            return false;
        }
    }
    
    // Here we display all registered users for admin purposes
    void displayAllUsers() {
        map<string, string> usersDatabase = loadUsersFromDatabase();
        
        cout << "\n" << string(40, '=') << "\n";
        cout << "           REGISTERED USERS\n";
        cout << string(40, '=') << "\n";
        
        if (usersDatabase.empty()) {
            cout << "No users registered yet.\n";
        } else {
            cout << "Total users: " << usersDatabase.size() << "\n\n";
            int userCount = 1;
            for (const auto& userPair : usersDatabase) {
                cout << userCount << ". " << userPair.first << "\n";
                userCount++;
            }
        }
        cout << string(40, '=') << "\n";
    }
    
    int getTotalUsers() {
        map<string, string> usersDatabase = loadUsersFromDatabase();
        return usersDatabase.size();
    }
};

// Here we display the main menu
void displayMainMenu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           USER AUTHENTICATION SYSTEM\n";
    cout << string(50, '=') << "\n";
    cout << "1. Register New User\n";
    cout << "2. Login Existing User\n";
    cout << "3. View All Registered Users\n";
    cout << "4. Exit\n";
    cout << string(50, '-') << "\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    UserAuthenticationSystem authenticationSystem;
    int userChoice;
    string inputLine;
    
    cout << "Welcome to the User Authentication System!\n";
    
    while (true) {
        displayMainMenu();
        getline(cin, inputLine);
        
        stringstream inputStream(inputLine);
        if (!(inputStream >> userChoice) || !inputStream.eof()) {
            cout << "Error: Please enter a valid number (1-4).\n";
            continue;
        }
        
        switch (userChoice) {
            case 1: {
                authenticationSystem.registerUser();
                break;
            }
            
            case 2: {
                authenticationSystem.loginUser();
                break;
            }
            
            case 3: {
                authenticationSystem.displayAllUsers();
                break;
            }
            
            case 4: {
                cout << "\nThank you for using the User Authentication System!\n";
                cout << "Total registered users: " << authenticationSystem.getTotalUsers() << "\n";
                cout << "Goodbye!\n";
                return 0;
            }
            
            default: {
                cout << "Error: Invalid choice. Please select option 1-4.\n";
                break;
            }
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore();
    }
    
    return 0;
}
