#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define MAX_OPTIONS 9

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
} datetime;

typedef struct {
    bool * crit;
    int * grd;
} criteria;

const char* labels[MAX_OPTIONS] = {
    "Assiduidade", 
    "Pontualidade", 
    "Iniciativa", 
    "Postura ético-profissional", 
    "Relacionamento em equipe", 
    "Espírito crítico", 
    "Comunicação", 
    "Habilidades e competências específicas", 
    "Participação nas atividades teóricas" };

// Function to validate the date format
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
    assert(input!=0);
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
    assert(choice == 'y' || choice == 'n');
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

// Function to toggle the checkbox state
void toggleCheckbox(bool checkboxes[], int index) {
    if (index == -1) {
        for (int i = 0; i < MAX_OPTIONS; i++) {
            checkboxes[i] = false;
        }
    }
    checkboxes[index] = !checkboxes[index];
}

// Function to print the checkbox states and labels
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
        assert(input<=10);
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

void freeCrit (criteria * c) {
    free((*c).crit);
    free((*c).grd);
}

void getActivity (char ** name) {
    printf("\033[2J\033[H");
    printf("ACTIVITY\n");
    printf("Enter the name or identifier for the activity (up to 50 characters): ");
    char input[51]; //50 chars + 1 end of line
    fgets(input, sizeof(input), stdin);
    //getchar();
    printf("%s", input);

    while (strlen(input) < 1) {
        printf("Invalid activity length. Insert at least one character.\n");
        printf("Enter the name or identifier for the activity (up to 50 characters): ");

        fgets(input, sizeof(input), stdin);
        //getchar();
    }

    // Remove the newline character from the input buffer
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    //printf("%s", input);
    char w = getchar();
    *name = (char*)malloc(strlen(input) + 1);
    strcpy(*name, input);
}

int main() {
    // Clear the terminal screen    
    printf("\033[2J\033[H");
    printf("New Feedback\n");

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

    freeCrit(&criteria);
    free(activity);

    getchar();

    // Clear the terminal screen
    //printf("\033[2J\033[H");

    return 0;
}