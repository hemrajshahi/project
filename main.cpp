#include <iostream>
#include <sqlite3.h>
#include <cmath>

// Function to create necessary tables
void createTables(sqlite3* db) {
    const char* createUserTableSQL = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );
    )";

    const char* createBMITableSQL = R"(
        CREATE TABLE IF NOT EXISTS bmi_records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            weight REAL,
            height REAL,
            gender TEXT,
            bmi REAL,
            body_category TEXT
        );
    )";

    char* errorMessage;
    int result = sqlite3_exec(db, createUserTableSQL, 0, 0, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "Error creating users table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    result = sqlite3_exec(db, createBMITableSQL, 0, 0, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "Error creating BMI records table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

// Function to register a user
void registerUser(sqlite3* db) {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    std::string sql = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "');";
    char* errorMessage;
    int result = sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "Error registering user: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "User registered successfully!" << std::endl;
    }
}

// Function to log in a user
bool loginUser(sqlite3* db) {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    std::string sql = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "';";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (result != SQLITE_OK) {
        std::cerr << "Error logging in: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        std::cout << "Login successful!" << std::endl;
        sqlite3_finalize(stmt);
        return true;
    } else {
        std::cout << "Invalid username or password." << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
}

// Function to display a quote
void displayQuote() {
    std::cout << "Quote of the Day: \"The only bad workout is the one that didn't happen.\" - Anonymous" << std::endl;
}

// Function to calculate BMI
void calculateBMI(sqlite3* db) {
    double weight, height;
    std::string gender;
    std::cout << "Enter weight (kg): ";
    std::cin >> weight;
    std::cout << "Enter height (cm): ";
    std::cin >> height;
    std::cout << "Enter gender (M/F): ";
    std::cin >> gender;

    height /= 100; // Convert height to meters
    double bmi = weight / (height * height);
    std::cout << "Your BMI is: " << bmi << std::endl;

    std::string bodyCategory;
    if (bmi < 18.5) {
        bodyCategory = "Underweight";
    } else if (bmi < 25) {
        bodyCategory = "Normal weight";
    } else if (bmi < 30) {
        bodyCategory = "Overweight";
    } else {
        bodyCategory = "Obesity";
    }

    std::cout << "Body Mass Index Scale" << std::endl;
    std::cout << "Underweight: less than 18.5" << std::endl;
    std::cout << "Normal weight: 18.5 - 25" << std::endl;
    std::cout << "Overweight: 25 - 30" << std::endl;
    std::cout << "Obesity: greater than 30" << std::endl;
    std::cout << "Body Category: " << bodyCategory << std::endl;

    // Example advice based on BMI
    if (bmi < 18.5) {
        std::cout << "Health Advice: You are underweight. Consider eating more nutritious food and consulting a healthcare provider." << std::endl;
    } else if (bmi < 25) {
        std::cout << "Health Advice: You have a normal weight. Keep maintaining a balanced diet and regular exercise." << std::endl;
    } else if (bmi < 30) {
        std::cout << "Health Advice: You are overweight. Consider a balanced diet and increasing physical activity." << std::endl;
    } else {
        std::cout << "Health Advice: You are obese. Consult a healthcare provider for a suitable weight loss plan." << std::endl;
    }

    // Save BMI to database (example)
    std::string sql = "INSERT INTO bmi_records (weight, height, gender, bmi, body_category) VALUES (" +
                      std::to_string(weight) + ", " + std::to_string(height * 100) + ", '" + gender + "', " +
                      std::to_string(bmi) + ", '" + bodyCategory + "');";
    char* errorMessage;
    int result = sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "Error recording BMI: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "BMI recorded successfully!" << std::endl;
    }
}

// Function to display BMI options
void displayBMIOptions(sqlite3* db) {
    int choice;
    while (true) {
        std::cout << "1. Calculate BMI\n2. Logout\nEnter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            calculateBMI(db);
        } else if (choice == 2) {
            break;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

int main() {
    sqlite3* db;
    int result = sqlite3_open("fitnessapp.db", &db);
    if (result) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Opened database successfully" << std::endl;
    }

    createTables(db);

    while (true) {
        int choice;
        std::cout << "1. Register\n2. Login\n3. Exit\nEnter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            registerUser(db);
        } else if (choice == 2) {
            if (loginUser(db)) {
                displayQuote();
                displayBMIOptions(db);
                break;
            }
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }

    sqlite3_close(db);
    return 0;
}
