#ifndef READ_PORT_H
#define READ_PORT_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <libserialport.h>
#include <sqlite3.h>

#define BUFFER_SIZE 10
#define SERIAL_PORT "/dev/ttyACM0"

extern char uid[BUFFER_SIZE];

int read_input();
void read_from_port(void* arg);

#endif /* READ_PORT_H */