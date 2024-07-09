#include <iostream>
#include <cmath>

class BMICalculator {
public:
    double weight;
    double height;

    BMICalculator(double w, double h) : weight(w), height(h) {}

    double calculateBMI() {
        double heightInMeters = height / 100;
        return weight / (heightInMeters * heightInMeters);
    }

    void displayBMI() {
        double bmi = calculateBMI();
        std::cout << "Your BMI is: " << bmi << std::endl;
        if (bmi < 18.5)
            std::cout << "Underweight: less than 18.5\n";
        else if (bmi < 25)
            std::cout << "Normal weight: 18.5 - 25\n";
        else if (bmi < 30)
            std::cout << "Overweight: 25 - 30\n";
        else
            std::cout << "Obesity: greater than 30\n";
    }

    std::string getBodyType() {
        double bmi = calculateBMI();
        if (bmi < 18.5)
            return "ectomorph";
        else if (bmi < 25)
            return "mesomorph";
        else
            return "endomorph";
    }
};

int main() {
    double weight, height;
    std::cout << "Enter your weight (kg): ";
    std::cin >> weight;
    std::cout << "Enter your height (cm): ";
    std::cin >> height;

    BMICalculator bmiCalc(weight, height);
    bmiCalc.displayBMI();
    std::string bodyType = bmiCalc.getBodyType();
    std::cout << "Your body type is: " << bodyType << std::endl;

    return 0;
}

