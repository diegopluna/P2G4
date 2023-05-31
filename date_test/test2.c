#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} DateTime;

typedef struct {
    DateTime start;
    DateTime end;
} Event;

void findIntersectingEvents(Event* events1, int size1, Event* events2, int size2) {
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            // Check for overlap between events
            if (events1[i].start.year <= events2[j].end.year &&
                events1[i].end.year >= events2[j].start.year &&
                events1[i].start.month <= events2[j].end.month &&
                events1[i].end.month >= events2[j].start.month &&
                events1[i].start.day <= events2[j].end.day &&
                events1[i].end.day >= events2[j].start.day &&
                events1[i].start.hour <= events2[j].end.hour &&
                events1[i].end.hour >= events2[j].start.hour &&
                events1[i].start.minute <= events2[j].end.minute &&
                events1[i].end.minute >= events2[j].start.minute) {
                // Print the intersecting event
                printf("Intersecting Event:\n");
                printf("Event 1: Start: %04d-%02d-%02d %02d:%02d, End: %04d-%02d-%02d %02d:%02d\n",
                       events1[i].start.year, events1[i].start.month, events1[i].start.day,
                       events1[i].start.hour, events1[i].start.minute,
                       events1[i].end.year, events1[i].end.month, events1[i].end.day,
                       events1[i].end.hour, events1[i].end.minute);
                printf("Event 2: Start: %04d-%02d-%02d %02d:%02d, End: %04d-%02d-%02d %02d:%02d\n",
                       events2[j].start.year, events2[j].start.month, events2[j].start.day,
                       events2[j].start.hour, events2[j].start.minute,
                       events2[j].end.year, events2[j].end.month, events2[j].end.day,
                       events2[j].end.hour, events2[j].end.minute);
                printf("---------------------------\n");
            }
        }
    }
}

int main() {
    // Define the events
    Event events1[] = {
        { {2023, 1, 1, 10, 0}, {2023, 1, 1, 12, 0} },
        { {2023, 1, 1, 14, 0}, {2023, 1, 1, 16, 0} },
        { {2023, 1, 2, 9, 0},  {2023, 1, 2, 12, 0} }
    };

    Event events2[] = {
        { {2023, 1, 1, 11, 0}, {2023, 1, 1, 13, 0} },
        { {2023, 1, 1, 15, 0}, {2023, 1, 1, 17, 0} },
        { {2023, 1, 2, 10, 0}, {2023, 1, 2, 13, 0} }
    };

    int size1 = sizeof(events1) / sizeof(events1[0]);
    int size2 = sizeof(events2) / sizeof(events2[0]);

    findIntersectingEvents(events1, size1, events2, size2);

    return 0;
}