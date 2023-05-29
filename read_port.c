#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h> //defines ssize_t
#include <libserialport.h>
#include <sqlite3.h>

#define BUFFER_SIZE 10
#define SERIAL_PORT "/dev/ttyACM0" //adapt, improvise, overcome (must have multiple options to correspond to different systems)

char uid[BUFFER_SIZE];

// Thread function for reading and validating input
void* read_from_port(void* arg) {
    //connect to the sqlite DB
    sqlite3 *db;
    int rc = sqlite3_open("database.db", &db);
    
    // Open the serial port
    struct sp_port* serialPort;
    sp_get_port_by_name(SERIAL_PORT, &serialPort);
    sp_open(serialPort, SP_MODE_READ);
    
    // Configure the serial port
    sp_set_baudrate(serialPort, 9600); // Set baud rate to 9600 (adjust as needed)
    sp_set_bits(serialPort, 8);        // Set data bits to 8
    sp_set_parity(serialPort, SP_PARITY_NONE); // Disable parity
    sp_set_stopbits(serialPort, 1);    // Set stop bits to 1
    sp_set_flowcontrol(serialPort, SP_FLOWCONTROL_NONE); // Disable flow control
    
    // Continuously read input from serial until stopped
    while (true) {
        sp_flush(serialPort, SP_BUF_INPUT);

        //char *inputBuffer = (char*)malloc(BUFFER_SIZE*sizeof(char)), input[9];
        char inputBuffer[BUFFER_SIZE];
        // Read input from serial
        ssize_t bytesRead = sp_blocking_read(serialPort, inputBuffer, BUFFER_SIZE - 1, 0);
        if (bytesRead == -1) {
            fprintf(stderr, "Failed to read from serial port\n");
            break;
        }
        
        inputBuffer[bytesRead-1] = '\0'; // Null-terminate the input
        
        // Check if the input is a valid UID
        bool isValid = true;
        for (int i = 0; i < 8; i++) {
            char currentChar = inputBuffer[i];
            if (!((currentChar >= '0' && currentChar <= '9') ||
                  (currentChar >= 'a' && currentChar <= 'f') ||
                  (currentChar >= 'A' && currentChar <= 'F'))) {
                isValid = false;
                break;
            }
        }

        //strcpy(uid, inputBuffer);
        
        // If the input is valid, print it
        if (isValid) {
            printf("Valid UID: %s\n", inputBuffer);
            strcpy(uid, inputBuffer);
        } else {
            printf("Invalid UID\n");
        }
    }
    
    sp_close(serialPort);
    sp_free_port(serialPort);
    return NULL;
}

int read_input() {
    pthread_t thread;
    
    // Create a thread to read and validate input
    if (pthread_create(&thread, NULL, read_from_port, NULL) != 0) {
        fprintf(stderr, "Failed to create thread\n");
        return 1;
    }
    
    // Wait for the user to stop the program
    printf("Press Enter to stop...\n");
    getchar();

    //printf("%s\n", uid);
    
    // Cancel the thread and wait for it to finish
    pthread_cancel(thread);
    pthread_join(thread, NULL);
    
    return 0;
}