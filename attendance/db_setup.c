#include <stdio.h>
#include <sqlite3.h>

int main() {
    // Connect to the SQLite database
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    printf("Database opened successfully.\n");

    // Create the "users" table
    char createUserTable[] = "CREATE TABLE IF NOT EXISTS users ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "uid TEXT NOT NULL UNIQUE,"
                             "name TEXT NOT NULL);";

    rc = sqlite3_exec(db, createUserTable, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    printf("Table 'users' created successfully.\n");

    // Create the "entries" table
    char createEntryTable[] = "CREATE TABLE IF NOT EXISTS entries ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "user_id TEXT NOT NULL,"
                              "datetime TEXT NOT NULL,"
                              "is_arrival INTEGER NOT NULL);";

    rc = sqlite3_exec(db, createEntryTable, 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    printf("Table 'entries' created successfully.\n");

    char fillFirstUser[] = "INSERT INTO users (uid, name) VALUES ('5312F411', 'Fernanda Alves da Silva');";

    rc = sqlite3_exec(db, fillFirstUser, 0, 0, 0);

    char fillSecondUser[] = "INSERT INTO users (uid, name) VALUES ('00000000', 'Felipe Costa Neto');";

    rc = sqlite3_exec(db, fillSecondUser, 0, 0, 0);

    printf("Populated table with second user.");

    // Close the database connection
    sqlite3_close(db);

    return 0;
}