#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <sqlite3.h>
#include <time.h>

#define MAX_LENGTH 12 //11+1

// Global variables for synchronization
pthread_mutex_t mutex;
bool isDataAvailable = false;
char inputData[MAX_LENGTH];
int id;
//char is_arrival = '1';

// Thread function for reading from serial port
void* serialThread(void* arg) {
    while (true) {
        char input[MAX_LENGTH];
        // Read input from serial port and store it in 'input' variable
        fgets(input, MAX_LENGTH, stdin);

        // Acquire the mutex lock
        pthread_mutex_lock(&mutex);

        // Copy the input to global variables
        strncpy(inputData, input, MAX_LENGTH);
        isDataAvailable = true;

        // Release the mutex lock
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Function to handle SQLite callback for SELECT queries
int selectCallback(void* data, int argc, char** argv, char** ColName) {
    // Process the SELECT query results here
    // You can access the query results using argv[] array
    id = atoi(argv[0]);
    const char* name = argv[2];

    printf("Nome do residente: %s\n", name);

    return 0;
}

int main() {
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create the serial thread
    pthread_t serialThreadId;
    pthread_create(&serialThreadId, NULL, serialThread, NULL);

    // Connect to the SQLite database
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    printf("Database opened successfully.\n");

    while (true) {
        // Acquire the mutex lock
        pthread_mutex_lock(&mutex);

        // Check if new data is available
        if (isDataAvailable) {
            char input[MAX_LENGTH];
            strncpy(input, inputData, MAX_LENGTH);

            // Reset the flag and release the mutex lock
            isDataAvailable = false;
            pthread_mutex_unlock(&mutex);

            // Trim the trailing newline character
            if (input[strlen(input) - 1] == '\n')
                input[strlen(input) - 1] = '\0';

            // Prepare the SELECT statement
            char query[MAX_LENGTH + 50];
            snprintf(query, sizeof(query), "SELECT * FROM users WHERE uid = '%s';", input);

            // Execute the SELECT statement
            rc = sqlite3_exec(db, query, selectCallback, 0, 0);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return 1;
            }

            printf("Query executed successfully.\n");

            // Get the current date and time
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            char datetime[MAX_LENGTH];
            strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", timeinfo);

            // Prepare the INSERT statement
            snprintf(query, sizeof(query), "INSERT INTO entries (user_id, datetime, is_arrival) VALUES (%d, '%s', 1);", id, datetime);
            
            // Execute the INSERT statement
            rc = sqlite3_exec(db, query, 0, 0, 0);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return 1;
            }

            id = 0;

            printf("Entry added successfully.\n");
        }

        // Release the mutex lock
        pthread_mutex_unlock(&mutex);
    }

    // Close the database connection
    sqlite3_close(db);

    // Cleanup
    pthread_mutex_destroy(&mutex);

    return 0;
}