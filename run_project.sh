#!/bin/bash

# Run the setup to initialize the database
./setup_db

# Run the registration
echo "Running registration..."
./register

# Run the login
echo "Running login..."
./login

# Run the BMI calculator
echo "Running BMI calculator..."
./bmi_calculator

# Run the workout planning
echo "Running workout planning..."
./planning

