#include <stdio.h>
#include <string.h>
#include <time.h>

#define ROWS 26

typedef struct {
    int start;
    int end;
    int day[7];
} regular;

typedef struct {
    int day;
    int start;
    int duration;
} extra;

void setMonthHeaders(int year, int month, int matrix[ROWS][31]) {
    struct tm timeinfo = {0};  // Initialize struct tm

    // Set the desired month and year
    timeinfo.tm_year = year - 1900;  // Year offset from 1900
    timeinfo.tm_mon = month - 1;     // Month offset from January (0-based)

    // Get the first day of the month
    timeinfo.tm_mday = 1;
    mktime(&timeinfo);  // Normalize the time

    // Determine the number of days in the month
    int numDays = 31;
    while (numDays > 28) {
        timeinfo.tm_mday++;
        mktime(&timeinfo);
        if (timeinfo.tm_mon != month - 1) {
            numDays--;  // Adjust for the last day of the previous month
            break;
        }
    }

    // Set the first row of the matrix with date and day headers
    for (int i = 0; i < numDays; i++) {
        matrix[0][i] = i + 1;
    }

    // // Get the abbreviated day names
    // char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    // Set the second row of the matrix with abbreviated day names
    for (int i = 0; i < numDays; i++) {
        int day = (timeinfo.tm_wday + 5) % 7;
        matrix[1][i] = day;
        timeinfo.tm_mday++;
        mktime(&timeinfo);
    }
}

int day_in_arr (int * arr, int day) {

    if (arr[day] == 1) {
        return 1;
    }

    return 0;
}

int main() {
    int year, month;

    printf("Enter the year: ");
    scanf("%d", &year);
    printf("Enter the month: ");
    scanf("%d", &month);

    int matrix[ROWS][31] = {0};  // Initialize the matrix

    setMonthHeaders(year, month, matrix);

    int row_len = 0;

    while (matrix[0][row_len] != 0) { //not working correctly
        row_len++;
    }

    FILE* file = fopen("data.csv", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Skip the first line containing column headers
    char line[1024];
    fgets(line, sizeof(line), file);
    regular regulars[7];
    int removal[32];
    extra extras[32];

    int ct = 0;

    while (1) {
        fgets(line, sizeof(line), file);
        
        if (line[0] == '*') {
            break;
        }

        int len = strlen(line);

        for (int i = 0; i < len - 1; i++) {
            if (line[i] == 'x') {
                line[i] = '1';
            } else if (line[i] == ' ') {
                line[i] = '0';
            }
        }

        // for (int i = 0; i < len - 1; i++) {
        //     if (line[i] == ',' && line[i+1] == ',') {
        //         char *c = &line[len -1];
        //         while (c != &line[i]) {
        //             *(c + 1) = *c;
        //             c--;
        //         }
        //         line[i+1] = '0';
        //     }
        // }

        int scanned = sscanf(line, "%dh,%dh,%d,%d,%d,%d,%d,%d,%d",
            &regulars[ct].start, &regulars[ct].end,
            &regulars[ct].day[0], &regulars[ct].day[1], &regulars[ct].day[2],
            &regulars[ct].day[3], &regulars[ct].day[4], &regulars[ct].day[5], &regulars[ct].day[6]);
        
        ct++;
    }

    fgets(line, sizeof(line), file); //skip line containing exclusion header

    int rem = 0;

    while (1) {
        fgets(line, sizeof(line), file);
        
        if (line[0] == '*') {
            removal[rem + 1] = 0;
            break;
        }

        char * token = strtok(line, ",");

        sscanf(token, "%d", &removal[rem]);

        rem++;
    }

    int ex = 0;

    fgets(line, sizeof(line), file); //skip line containing extra header

    while (1) {
        fgets(line, sizeof(line), file);

        if (line[0] == '*') {
            //removal[rem + 1] = 0;
            break;
        }

        sscanf(line, "%d,%dh,%dh", &extras[ex].day, &extras[ex].start, &extras[ex].duration);

        ex++;
    }

    printf("\n");

    //printf("start hour: %d, end hour: %d", regular.start, regular.end);

    //attribute the hour intervals where the regular service time is expected with value 1
    for (int k = 0; k < ct; k++) {
        for (int i = 0; i < row_len; i++) {
            if (day_in_arr(regulars[k].day, matrix[1][i]) == 1) {
                for (int j = regulars[k].start + 1; j < regulars[k].end + 1; j++) {
                matrix[j][i] = 1;
                }
            }
        }
    }
    
    //printf("rem = %d", rem);

    for (int i = 0; i < rem; i++) {
        for (int j = 2; j < ROWS; j++) {
            matrix[j][removal[i]] = 0;
        }
    }

    for (int i = 0; i < ex; i++) {
        int da = extras[i].day, du = extras[i].duration, st = extras[i].start;

        while (du > 0) {
            matrix[st][da] = 1;
            st++;
            if (st > 25) {
                st = 2;
                da++;
            }
            du--;
        }
    }

    // Print the matrix (for demonstration purposes)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < row_len; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}