#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

sqlite3* db;
char* zErrMsg = 0;
int rc;
const char* sql;
sqlite3_stmt* stmt;

void openDatabase() {
    rc = sqlite3_open("fitness_app.db", &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        exit(0);
    } else {
        cout << "Opened database successfully" << endl;
    }
}

void createTables() {
    sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS bmi_records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            weight REAL,
            height REAL,
            gender TEXT,
            bmi REAL,
            body_type TEXT,
            body_category TEXT,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );
    )";
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << "Tables created successfully" << endl;
    }
}

bool userExists(const string& username) {
    string sql = "SELECT COUNT(*) FROM users WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            return count > 0;
        }
    }
    sqlite3_finalize(stmt);
    return false;
}

bool authenticateUser(const string& username, const string& password) {
    string sql = "SELECT password FROM users WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            string stored_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);
            return stored_password == password;
        }
    }
    sqlite3_finalize(stmt);
    return false;
}

void registerUser() {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;

    if (userExists(username)) {
        cout << "Username already exists. Try a different username." << endl;
        return;
    }

    string sql = "INSERT INTO users (username, password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        cout << "User registered successfully!" << endl;
    } else {
        cerr << "Failed to register user: " << sqlite3_errmsg(db) << endl;
    }
}

int loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (authenticateUser(username, password)) {
        cout << "Login successful!" << endl;
        string sql = "SELECT id FROM users WHERE username = ?";
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                int user_id = sqlite3_column_int(stmt, 0);
                sqlite3_finalize(stmt);
                return user_id;
            }
        }
    } else {
        cout << "Invalid username or password." << endl;
    }
    return -1;
}

double calculateBMI(double weight, double height_cm) {
    double height_m = height_cm / 100.0; // Convert cm to meters
    return weight / (height_m * height_m);
}

string getBodyType(double bmi) {
    if (bmi < 18.5) {
        return "Underweight";
    } else if (bmi < 25) {
        return "Normal weight";
    } else if (bmi < 30) {
        return "Overweight";
    } else {
        return "Obesity";
    }
}

string getBodyCategory(double bmi) {
    if (bmi < 18.5) {
        return "Ectomorph";
    } else if (bmi >= 18.5 && bmi < 25) {
        return "Mesomorph";
    } else {
        return "Endomorph";
    }
}

void provideAdvice(double bmi, const string& body_category) {
    cout << "Your BMI is: " << bmi << endl;
    cout << "Body Type: " << getBodyType(bmi) << endl;
    cout << "Body Category: " << body_category << endl;

    if (body_category == "Ectomorph") {
        cout << "Health Advice: Focus on strength training and a calorie-dense diet to gain weight." << endl;
    } else if (body_category == "Mesomorph") {
        cout << "Health Advice: Maintain a balanced diet and regular exercise to stay fit." << endl;
    } else if (body_category == "Endomorph") {
        cout << "Health Advice: Prioritize cardiovascular exercise and a healthy diet to lose weight." << endl;
    }
}

void displayBMIScale() {
    cout << "Body Mass Index Scale" << endl;
    cout << "Underweight: less than 18.5" << endl;
    cout << "Normal weight: 18.5 - 25" << endl;
    cout << "Overweight: 25 - 30" << endl;
    cout << "Obesity: greater than 30" << endl;
}

void calculateAndRecordBMI(int user_id) {
    double weight, height;
    string gender;
    cout << "Enter weight (kg): ";
    cin >> weight;
    cout << "Enter height (cm): ";
    cin >> height;
    cout << "Enter gender (M/F): ";
    cin >> gender;

    double bmi = calculateBMI(weight, height);
    string body_category = getBodyCategory(bmi);
    provideAdvice(bmi, body_category);

    string sql = "INSERT INTO bmi_records (user_id, weight, height, gender, bmi, body_type, body_category) VALUES (?, ?, ?, ?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_double(stmt, 2, weight);
        sqlite3_bind_double(stmt, 3, height);
        sqlite3_bind_text(stmt, 4, gender.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 5, bmi);
        sqlite3_bind_text(stmt, 6, getBodyType(bmi).c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 7, body_category.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        cout << "BMI recorded successfully!" << endl;
    } else {
        cerr << "Failed to record BMI: " << sqlite3_errmsg(db) << endl;
    }
}

void displayQuote() {
    cout << "Welcome to the Fitness App!" << endl;
    cout << "Quote of the Day: \"The only bad workout is the one that didn't happen.\" - Anonymous" << endl;
}

int main() {
    openDatabase();
    createTables();

    int choice;
    int user_id = -1;
    while (true) {
        if (user_id == -1) {
            cout << "1. Register\n2. Login\n3. Exit\nEnter choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    user_id = loginUser();
                    if (user_id != -1) {
                        displayQuote();
                    }
                    break;
                case 3:
                    sqlite3_close(db);
                    return 0;
                default:
                    cout << "Invalid choice, please try again." << endl;
            }
        } else {
            cout << "1. Calculate BMI\n2. Logout\nEnter choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    displayBMIScale();
                    calculateAndRecordBMI(user_id);
                    break;
                case 2:
                    user_id = -1;
                    cout << "Logged out successfully!" << endl;
                    break;
                default:
                    cout << "Invalid choice, please try again." << endl;
            }
        }
    }
}

