#include <iostream>
#include <sqlite3.h>
#include <string>

class User {
public:
    std::string username;
    std::string password;

    User(std::string uname, std::string pwd) : username(uname), password(pwd) {}

    bool loginUser() {
        sqlite3* DB;
        sqlite3_stmt* stmt;
        int exit = sqlite3_open("fitness.db", &DB);

        std::string sql = "SELECT * FROM USERS WHERE USERNAME = ? AND PASSWORD = ?;";
        
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            sqlite3_close(DB);
            return true;
        }
        
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return false;
    }
};

int main() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    
    User user(username, password);

    if (user.loginUser()) {
        std::cout << "Login successful! Let's get started.\n";
        std::cout << "Here's a quote to motivate you: 'The journey of a thousand miles begins with one step.'\n";
    } else {
        std::cout << "Invalid username or password.\n";
    }
    
    return 0;
}

