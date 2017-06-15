#include <iostream>
#include <string>
#include "sqlite3.h"

int callback(void *NotUsed, int argc, char **argv, char **colName) {
    for (int i = 0; i < argc; i++) {
        std::cout << colName[i] << ": " << argv[i] << std::endl;
    }
    std::cout << "\n" << std::endl;
    return 0;
}

void addExercise() {}

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
                                        "exercise_date, DATE, " \
                                        "date_added DATETIME DEFAULT CURRENT_TIMESTAMP);";
    rc = sqlite3_exec(db, createTableStatement.c_str(), callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "Error executing supplied statement: " << errMsg << std::endl;
        return 1;
    }
    
    rc = sqlite3_exec(db, argv[1], callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cout << "Error executing supplied statement: " << errMsg << std::endl;
        return 1;
    }

    std::cout << "Completed statement execution: "  << argv[1] << std::endl;
    std::cout << "Closing database connection" << std::endl;
    sqlite3_close(db);
}

// "CREATE TABLE IF NOT EXISTS exercises (id INTEGER PRIMARY KEY, name TEXT, reps INTEGER, sets INTEGER, weight INTEGER, date_added DATETIME DEFAULT CURRENT_TIMESTAMP);"
// "INSERT INTO exercises (name, reps, sets, weight) VALUES ('Shoulder press', 6, 4, 45);"
// "SELECT * FROM exercises"
// g++ .\workouts.cpp  -o ..\workouts.exe -I "C:\Sqlite3\" -L "C:\Sqlite3\" -lsqlite3

// todo - create cmd line interface for adding new workouts and other tasks!