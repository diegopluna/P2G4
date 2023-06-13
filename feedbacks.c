//
// Created by diego on 6/13/2023.
//

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "feedbacks.h"
#include "sqlite3.h"

#define MAX_OPTIONS 9
//
const char* labels[MAX_OPTIONS] = {
        "Assiduidade",
        "Pontualidade",
        "Iniciativa",
        "Postura etico-profissional",
        "Relacionamento em equipe",
        "Espirito critico",
        "Comunicacao",
        "Habilidades e competencias especificas",
        "Participacao nas atividades teoricas"
};

bool isValidDate(datetime dt) {
    if (dt.year < 0 || dt.month < 1 || dt.month > 12 || dt.day < 1) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0)) {
        daysInMonth[1] = 29; // Leap year
    }

    return dt.day <= daysInMonth[dt.month - 1];
}

datetime getDate () {
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    datetime dt;

    dt.day = localTime->tm_mday;
    dt.month = localTime->tm_mon + 1;
    dt.year = localTime->tm_year + 1900;
    dt.hour = -1;
    dt.minute = -1;

    printf("Default date: %02d/%02d/%04d\n", dt.day, dt.month, dt.year);

    printf("Enter day (press enter for default): ");
    char input[3];
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        getchar();
        sscanf(input, "%d", &dt.day);

        // Validate the day
        while (!isValidDate(dt) || dt.day > localTime->tm_mday) {
            printf("Invalid day format!\nMust be between.\n");
            printf("Enter day (press enter for default): ");
            fgets(input, sizeof(input), stdin);
            if (input[0] == '\n') {
                dt.day = localTime->tm_mday; // Set to default day
                break;
            }
            sscanf(input, "%d", &dt.day);
            getchar();
        }
    }

    // Check if the user wants to enter the time
    char choice;
    printf("Do you want to enter the time? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        printf("Enter time (hh:mm): ");
        scanf("%d:%d", &dt.hour, &dt.minute);

        // Validate the time
        while (dt.hour < 0 || dt.hour > 23 || dt.minute < 0 || dt.minute > 59) {
            printf("Invalid time format!\nMust be between 00:00 and 23:59.");
            scanf("%d:%d", &dt.hour, &dt.minute);
        }
    }

    printf("Date: %02d/%02d/%04d\n", dt.day, dt.month, dt.year);
    if (dt.hour != -1 && dt.minute != -1) {
        printf("Time: %02d:%02d\n", dt.hour, dt.minute);
    }


    return dt;
}

void toggleCheckbox(bool checkboxes[], int index) {
    if (index == -1) {
        for (int i = 0; i < MAX_OPTIONS; i++) {
            checkboxes[i] = false;
        }
    }
    checkboxes[index] = !checkboxes[index];
}

void printCheckboxes(bool checkboxes[], const char* labels[], int numOptions) {
    printf("Select the indices (1-9) to toggle the respective checkboxes.\nPressing 0 defaults to all unchecked.\nPress d to exit.\n");
    for (int i = 0; i < numOptions; i++) {
        printf("%d - [%c] %s\n", (i+1), checkboxes[i] ? 'X' : ' ', labels[i]);
    }
}

criteria getCriteria() {

    criteria ret;

    bool* checkboxes = malloc(MAX_OPTIONS * sizeof(bool));
    int* grades = malloc(MAX_OPTIONS * sizeof(int));

    // Initialize checkboxes and grades
    for (int i = 0; i < MAX_OPTIONS; i++) {
        checkboxes[i] = false;
        grades[i] = 0;
    }

    int numOptions = MAX_OPTIONS;

    // Clear the terminal screen
    printf("\033[2J\033[H");

    printf("CRITERIA");

    // Print the initial checkbox states
    //printCheckboxes(checkboxes, labels, numOptions);

    while (true) {
        // Wait for user input
        char input = getchar();

        // Check if the input corresponds to a valid option index
        int optionIndex = input - ('0'+1);
        if (optionIndex >= -1 && optionIndex < numOptions) {
            toggleCheckbox(checkboxes, optionIndex);
        }

        // Clear the terminal screen
        printf("\033[2J\033[H");

        // Print the updated checkbox states
        printCheckboxes(checkboxes, labels, numOptions);

        // Break the loop on pressing 'q' or 'Q'
        if (input == 'd' || input == 'D') {
            break;
        }
    }

    // for (int i = 0; i < MAX_OPTIONS; i++) {
    //     if (checkboxes[i] == true) {
    //         int grd_buffer;

    //         printf("Enter grade suggestion (0-10) for criterium %s (press enter for no suggestion): ", labels[i]);
    //         char input[3];
    //         fgets(input, sizeof(input), stdin);
    //         getchar();

    //         if (input[0] != '\n') {
    //         //if (strlen(input) > 1) {
    //             //getchar();
    //             sscanf(input, "%d", &grd_buffer);

    //             // Validate the grade
    //             while (grd_buffer < 0 || grd_buffer > 10) {
    //                 printf("Invalid grade!\nMust be between (0-10).\n");
    //                 printf("Enter grade (press enter for no suggestion): ");
    //                 fgets(input, sizeof(input), stdin);
    //                 if (input[0] == '\n') {
    //                     grades[i] = -1;
    //                     break;
    //                 }
    //                 sscanf(input, "%d", &grd_buffer);
    //                 getchar();
    //             }

    //             grades[i] = grd_buffer;
    //         } else {
    //             grades[i] = -1;
    //         }
    //     } else {
    //         grades[i] = -1;
    //     }
    // }

    for (int i = 0; i < MAX_OPTIONS; i++) {
        if (checkboxes[i] == true) {
            int grd_buffer;

            printf("Enter grade suggestion (0-10) for criterium %s (press enter for no suggestion): ", labels[i]);
            char input[4];
            scanf("%3s", input); // Read at most 3 characters (grade digits)
            if (input[0] != '\0') {
                sscanf(input, "%d", &grd_buffer);

                // Validate the grade
                while (grd_buffer < 0 || grd_buffer > 10) {
                    printf("Invalid grade!\nMust be between (0-10).\n");
                    printf("Enter grade (press enter for no suggestion): ");
                    scanf("%3s", input);
                    if (input[0] == '\0') {
                        grades[i] = -1;
                        break;
                    }
                    sscanf(input, "%d", &grd_buffer);
                }

                grades[i] = grd_buffer;
            } else {
                grades[i] = -1;
            }
            //getchar(); // Consume the newline character
        } else {
            grades[i] = -1;
        }
    }

    ret.crit = checkboxes;
    ret.grd = grades;

    return ret;
}


int getActivity(char** name) {
    printf("\033[2J\033[H");
    printf("ACTIVITY\n");
    printf("Enter the name or identifier for the activity (up to 50 characters): ");
    char input[51]; // 50 chars + 1 end of line
    fgets(input, sizeof(input), stdin);

    // Remove the newline character from the input buffer
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    } else {
        // Input exceeded the maximum allowed length
        printf("Invalid activity length. Maximum length exceeded.\n");
        return 0; // Return failure
    }

    while (strlen(input) < 1) {
        printf("Invalid activity length. Insert at least one character.\n");
        printf("Enter the name or identifier for the activity (up to 50 characters): ");
        fgets(input, sizeof(input), stdin);

        // Remove the newline character from the input buffer
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        } else {
            // Input exceeded the maximum allowed length
            printf("Invalid activity length. Maximum length exceeded.\n");
            return 0; // Return failure
        }
    }

    *name = (char*)malloc(strlen(input) + 1);
    if (*name == NULL) {
        // Memory allocation failed
        printf("Memory allocation failed.\n");
        return 0; // Return failure
    }
    strcpy(*name, input);

    return 1; // Return success
}

//int setFeedback() {
//    // Clear the terminal screen
//    printf("\033[2J\033[H");
//    printf("New Feedback\n");
//
//    datetime dt = getDate();
//
//    char * activity;
//    getActivity(&activity);
//
//    criteria criteria = getCriteria();
//
//    printf("Date: %02d/%02d/%04d\n", dt.day, dt.month, dt.year);
//    printf("Activity: %s\n", activity);
//    printf("Criteria:\n");
//    for (int i = 0; i < MAX_OPTIONS; i++) {
//        printf("%s: %d (grade suggestion: %d)\n", labels[i], criteria.crit[i], criteria.grd[i]);
//    }
//
//    freeCrit(&criteria);
//    free(activity);
//
//    getchar();
//
//    // Clear the terminal screen
//    //printf("\033[2J\033[H");
//
//    return 0;
//}

void feedbackScreen() {

    printf("\033[2J\033[H");
    printf("New Feedback\n");

    char email[100];
    fgets(email, sizeof(email), stdin);
    if (email[strlen(email) - 1] == '\n') {
        email[strlen(email) - 1] = '\0';
    }

    int userId = isUserExists(email);
    if (userId != -1) {
        printf("User exists. User ID: %d\n", userId);
    } else {
        printf("User does not exist.\n");
        return;
    }

    datetime dt = getDate();
    char * activity;
    getActivity(&activity);

    criteria criteria = getCriteria();

    printf("Date: %02d/%02d/%04d\n", dt.day, dt.month, dt.year);
    printf("Activity: %s\n", activity);
    printf("Criteria:\n");
    for (int i = 0; i < MAX_OPTIONS; i++) {
        printf("%s: %d (grade suggestion: %d)\n", labels[i], criteria.crit[i], criteria.grd[i]);
    }

    char confirm;
    printf("Save feedback? (Y/N): ");
    scanf(" %c", &confirm);

    if (confirm == 'Y' || confirm == 'y') {
        saveFeedbackToDatabase(dt, activity, criteria, userId);
        printf("Feedback saved successfully!\n");
    } else {
        printf("Feedback discarded.\n");
    }

//    freeCrit(&criteria);
    free(activity);

    getchar();

    // Clear the terminal screen
    //printf("\033[2J\033[H");

}

int initializeDatabase2() {
    sqlite3* db;
    int rc;

    // Open the database file (or create if it doesn't exist)
    rc = sqlite3_open("feedback.db", &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Create the feedback table
    const char* createTableQuery = "CREATE TABLE IF NOT EXISTS feedback ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "user_id INTEGER,"
                                   "datetime TEXT,"
                                   "activity TEXT,"
                                   "criteria TEXT"
                                   ");";
    rc = sqlite3_exec(db, createTableQuery, 0, 0, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Close the database connection
    sqlite3_close(db);

    return SQLITE_OK;
}

void saveFeedbackToDatabase(datetime dt, const char* activity, criteria criteria, int user_id) {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("feedback.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char datetimeStr[20];
    snprintf(datetimeStr, sizeof(datetimeStr), "%04d-%02d-%02d %02d:%02d",
             dt.year, dt.month, dt.day, dt.hour, dt.minute);

    char sql[500];
    snprintf(sql, sizeof(sql), "INSERT INTO feedback (user_id, datetime, activity, criteria) VALUES ('%d','%s', '%s', '",
             user_id, datetimeStr, activity);

    for (int i = 0; i < MAX_OPTIONS; i++) {
        if (criteria.crit[i]) {
            char critStr[10];
            snprintf(critStr, sizeof(critStr), "%d:%d", i, criteria.grd[i]);
            strncat(sql, critStr, sizeof(sql) - strlen(sql) - 1);
        }

        if (i != MAX_OPTIONS - 1 && criteria.crit[i]) {
            strncat(sql, ",", sizeof(sql) - strlen(sql) - 1);
        }
    }

    strncat(sql, "')", sizeof(sql) - strlen(sql) - 1);

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_close(db);
}

int isUserExists(const char *email) {
    sqlite3 *db;
    int rc = sqlite3_open("users.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Prepare the SELECT statement
    sqlite3_stmt *stmt;
    const char *query = "SELECT uid FROM users WHERE email = ?";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Bind the email parameter
    rc = sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Execute the SELECT statement
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        // User exists, retrieve the user ID
        int userId = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return userId;
    } else if (rc == SQLITE_DONE) {
        // User does not exist
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    } else {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
}

void retrieveAndPrintFeedbacks(int userId) {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("feedback.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char sql[100];
    snprintf(sql, sizeof(sql), "SELECT * FROM feedback WHERE user_id = %d;", userId);

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int feedbackId = sqlite3_column_int(stmt, 0);
        int userId = sqlite3_column_int(stmt, 1);
        const char *datetime = (const char *)sqlite3_column_text(stmt, 2);
        const char *activity = (const char *)sqlite3_column_text(stmt, 3);
        const char *criteria = (const char *)sqlite3_column_text(stmt, 4);

        printf("Feedback ID: %d\n", feedbackId);
        printf("User ID: %d\n", userId);
        printf("Datetime: %s\n", datetime);
        printf("Activity: %s\n", activity);
        printf("Criteria: %s\n", criteria);
        printf("------------------------------\n");
    }
    getchar();
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
