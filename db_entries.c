#include "read_port.h"
#include <stdbool.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int id;
    char name[100];
} User;

// Function to query the SQLite database and retrieve user information
bool getUserFromDatabase(const char* uid, User* user) {
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_stmt* stmt;
    const char* query = "SELECT id, name FROM users WHERE uid=?";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_bind_text(stmt, 1, uid, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        user->id = sqlite3_column_int(stmt, 0);
        strncpy(user->name, (const char*)sqlite3_column_text(stmt, 1), sizeof(user->name) - 1);
        user->name[sizeof(user->name) - 1] = '\0';
    } else if (rc == SQLITE_DONE) {
        fprintf(stderr, "User not found for UID: %s\n", uid);
    } else {
        fprintf(stderr, "Failed to retrieve user: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool getEntryType (User user) {
    int is_arrival;
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_stmt* stmt;
    const char* query = "SELECT is_arrival FROM entries WHERE user_id=? ORDER BY id DESC LIMIT 1";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_bind_int(stmt, 1, user.id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        is_arrival = sqlite3_column_int(stmt, 0);
    } else if (rc == SQLITE_DONE) {
        fprintf(stderr, "No previous entry; will set the next entry to arrival.\n");
        is_arrival = 1;
    } else {
        fprintf(stderr, "Failed to retrieve entry: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (is_arrival+1)%2;
}

bool setEntry (User user, bool is_arrival) {
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_stmt* stmt;
    const char* query = "INSERT INTO entries (user_id, datetime, is_arrival) VALUES (?, ?, ?)";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);

    char datetimeString[20];  // "yyyy-mm-dd hh:mm" + null terminator
    strftime(datetimeString, sizeof(datetimeString), "%Y-%m-%d %H:%M:%S", localTime);

    printf("Current datetime: %s\n", datetimeString);

    rc = sqlite3_bind_int(stmt, 1, user.id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }  

    rc = sqlite3_bind_text(stmt, 2, datetimeString, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_bind_int(stmt, 3, is_arrival);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("Succesfully created entry.");

}

// Function to handle the UID reading and database query
User retrieveUserFromUID(const char* uid) {
    User user;
    bool success = getUserFromDatabase(uid, &user);
    if (!success) {
        // Handle the error or return a default/empty user struct
    }
    return user;
}

int main () {
    User entry_user;

    read_input();

    getUserFromDatabase(uid, &entry_user);

    printf("id: %d, name: %s", entry_user.id, entry_user.name);

    bool is_arrival = getEntryType(entry_user);

    printf("próxima entrada: %d", is_arrival);

    setEntry(entry_user, is_arrival);

    return 0;
}