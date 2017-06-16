#include <iostream>
#include <algorithm>
#include <string>
#include "sqlite3.h"

int callback(void *NotUsed, int argc, char **argv, char **colName) {
    for (int i = 0; i < argc; i++) {
        std::cout << colName[i] << ": " << argv[i] << std::endl;
    }
    std::cout << "\n" << std::endl;
    return 0;
}

std::string allExercises() {
    return "SELECT * FROM exercises;";
}

std::string addExercise() {
    std::string name, date;
    int reps, sets, weight;
    std::cout << "Exercise name: ";
    std::cin >> name;
    
    std::cout << "Exercise reps: ";
    std::cin >> reps;

    std::cout << "Exercise sets: ";
    std::cin >> sets;

    std::cout << "Exercise weight: ";
    std::cin >> weight;

    std::cout << "Exercise date [yyyy-mm-dd]: ";
    std::cin >> date;
    return "INSERT INTO exercises (name, reps, sets, weight, exercise_date) VALUES ('"
            + name + '\'' + ", " + std::to_string(reps) + ", " + std::to_string(4) + ", "
            + std::to_string(0) + ", " + '\'' + date + '\'' + ");";
}

void getDayExercise() {}

int main(int argc, char *argv[]) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    if (argc < 2) {
        std::cout << "Please enter a SQL statement to execute" << std::endl;
        return 1;
    }
    
    rc = sqlite3_open("workouts.db", &db);
    if (rc) {
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Opened database successfully\n" << std::endl;
    }

    std::string createTableStatement = "CREATE TABLE IF NOT EXISTS exercises (" \
                                        "id INTEGER PRIMARY KEY, " \
                                        "name TEXT, " \
                                        "reps INTEGER, " \
                                        "sets INTEGER, " \
                                        "weight INTEGER, " \
                                        "exercise_date DATETIME, " \
                                        "date_added DATETIME DEFAULT CURRENT_TIMESTAMP);";
    rc = sqlite3_exec(db, createTableStatement.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "Error executing supplied statement: " << errMsg << std::endl;
        return 1;
    }

    std::string command = argv[1];
    std::string sqlStatement;
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    if (command == "all") {
        sqlStatement = allExercises();
    } else if (command == "add") {
        sqlStatement = addExercise();
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        return 1;
    }

    rc = sqlite3_exec(db, sqlStatement.c_str(), callback, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "Error executing supplied statement: " << errMsg << std::endl;
        return 1;
    }

    std::cout << "Completed statement execution: "  << sqlStatement << std::endl;
    std::cout << "Closing database connection" << std::endl;
    sqlite3_close(db);
}
