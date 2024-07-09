#include <iostream>
#include <string>
#include <sqlite3.h>

class WorkoutPlanner {
public:
    std::string bodyType;
    std::string username;

    void showCharacteristics() {
        if (bodyType == "ectomorph") {
            std::cout << "The Ectomorph Body Type" << std::endl;
            std::cout << "Characteristics:\n";
            std::cout << " - Thin and lean with a narrow frame\n";
            std::cout << " - Slender waist, narrow hips, and long limbs\n";
            std::cout << " - High endurance levels and fast metabolism\n";
        } else if (bodyType == "endomorph") {
            std::cout << "The Endomorph Body Type" << std::endl;
            std::cout << "Characteristics:\n";
            std::cout << " - Larger-boned with pear or hour-glass shape\n";
            std::cout << " - Small waist, narrow shoulders, and shorter limbs\n";
            std::cout << " - Tendency to gain weight easily with a slower metabolism\n";
        } else if (bodyType == "mesomorph") {
            std::cout << "The Mesomorph Body Type" << std::endl;
            std::cout << "Characteristics:\n";
            std::cout << " - Athletic, solid, and strong with a medium bone structure\n";
            std::cout << " - Broad shoulders and a relatively narrow waist\n";
            std::cout << " - Efficient metabolism for weight management\n";
        } else {
            std::cout << "Unknown body type.\n";
        }
    }

    void recommendNutrition() {
        if (bodyType == "ectomorph") {
            std::cout << "Nutrition advice for Ectomorph:\n";
            std::cout << " - Focus on high-calorie, nutrient-dense foods\n";
            std::cout << " - Include complex carbohydrates, lean proteins, and healthy fats\n";
        } else if (bodyType == "endomorph") {
            std::cout << "Nutrition advice for Endomorph:\n";
            std::cout << " - Focus on portion control and balanced meals\n";
            std::cout << " - Limit refined sugars and carbohydrates\n";
        } else if (bodyType == "mesomorph") {
            std::cout << "Nutrition advice for Mesomorph:\n";
            std::cout << " - Balanced diet with moderate carbohydrates, proteins, and fats\n";
            std::cout << " - Emphasize lean proteins and whole grains\n";
        } else {
            std::cout << "Unknown body type.\n";
        }
    }

    void recommendWorkout() {
        if (bodyType == "ectomorph") {
            std::cout << "Best Workouts for Ectomorphs:\n";
            std::cout << " - Endurance sports like distance running or cycling\n";
            std::cout << " - Cross-country skiing, marathons, or triathlons\n";
            std::cout << " - Circuit training and body sculpting\n";
        } else if (bodyType == "endomorph") {
            std::cout << "Best Workouts for Endomorphs:\n";
            std::cout << " - Cardio exercises like running, swimming, and cycling\n";
            std::cout << " - Powerlifting and high-intensity circuit training\n";
            std::cout << " - Incorporate plyometrics and bodyweight exercises\n";
        } else if (bodyType == "mesomorph") {
            std::cout << "Best Workouts for Mesomorphs:\n";
            std::cout << " - Strength training with weights and high-intensity workouts\n";
            std::cout << " - Interval training and power-based sports\n";
            std::cout << " - Alternating steady-state cardio with HIIT\n";
        } else {
            std::cout << "Unknown body type.\n";
        }
    }

    void saveToDatabase(sqlite3* db) {
        std::string sql = "INSERT INTO workout_plans (username, body_type, recommendations) VALUES ('" + username + "', '" + bodyType + "', 'Recommended workout plan for " + bodyType + "');";
        char* errMsg = 0;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }
};

void planWorkout(const std::string& username) {
    WorkoutPlanner planner;
    planner.username = username;

    std::cout << "Enter your body type (ectomorph/endomorph/mesomorph): ";
    std::cin >> planner.bodyType;

    int choice;
    while (true) {
        std::cout << "1. Characteristics\n2. Nutrition and Diet Advice\n3. Exercise\nChoose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            planner.showCharacteristics();
            break;
        case 2:
            planner.recommendNutrition();
            break;
        case 3:
            planner.recommendWorkout();
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }

        std::cout << "Do you want to continue? (yes/no): ";
        std::string cont;
        std::cin >> cont;
        if (cont == "no")
            break;
    }

    sqlite3* db;
    int rc = sqlite3_open("fitness.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    planner.saveToDatabase(db);
    sqlite3_close(db);
}

int main() {
    std::string username;
    std::cout << "Enter your username: ";
    std::cin >> username;

    planWorkout(username);

    return 0;
}


