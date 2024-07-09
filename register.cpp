#include <iostream>
#include <sqlite3.h>
#include <string>

class User {
public:
    std::string username;
    std::string password;

    User(std::string uname, std::string pwd) : username(uname), password(pwd) {}

    void registerUser() {
        sqlite3* DB;
        char* errorMessage;
        int exit = sqlite3_open("fitness.db", &DB);
        
        std::string sql = "CREATE TABLE IF NOT EXISTS USERS("
                          "USERNAME TEXT PRIMARY KEY NOT NULL, "
                          "PASSWORD TEXT NOT NULL);";
                          
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMessage);
        
        sql = "INSERT INTO USERS (USERNAME, PASSWORD) VALUES ('" + username + "', '" + password + "');";
        
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMessage);
        
        if (exit != SQLITE_OK) {
            std::cerr << "Error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        } else {
            std::cout << "User registered successfully!" << std::endl;
        }
        
        sqlite3_close(DB);
    }
};

int main() {
    std::string username, password;
    std::cout << "Enter a username: ";
    std::cin >> username;
    std::cout << "Enter a password: ";
    std::cin >> password;
    
    User user(username, password);
    user.registerUser();
    
    return 0;
}

