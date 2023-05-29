#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sqlite3.h>

#define MAX_ROWS 24
#define MAX_COLS 31

typedef struct {
    int user_id;
    char data[(MAX_ROWS * MAX_COLS * 2) + 1];  // Maximum length of the data string
} MatrixEntry;

int main() {
    FILE* file = fopen("data.csv", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Skip the first line containing column headers
    char line[1024];
    fgets(line, sizeof(line), file);

    // Read the whole table into a string
    char dataString[(MAX_ROWS * MAX_COLS * 2) + MAX_ROWS];  // Additional space for separators and null terminator
    dataString[0] = '\0';  // Initialize the string

    //strcat(dataString, line + 1);

    while (fgets(line, sizeof(line), file)) {
        strcat(dataString, line + 9);
    }

    fclose(file);

    for (int i = 0; i)

    // Open the SQLite database connection
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Create the table if it doesn't exist
    const char* createTableQuery = "CREATE TABLE IF NOT EXISTS matrix (user_id INTEGER, data TEXT)";
    rc = sqlite3_exec(db, createTableQuery, NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Prepare the insert statement
    sqlite3_stmt* stmt;
    const char* insertQuery = "INSERT INTO matrix (user_id, data) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }


    MatrixEntry entry;
    entry.user_id = 1;
    strcpy(entry.data, dataString);

    // Insert the parsed data into the SQLite table
    rc = sqlite3_bind_int(stmt, 1, entry.user_id);
    rc |= sqlite3_bind_text(stmt, 2, entry.data, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameters: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}