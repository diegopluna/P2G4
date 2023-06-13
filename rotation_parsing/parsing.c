#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>

#define ROWS 26
#define COLS 31
#define MAX_MATRICES 100

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

typedef struct {
    int id;
    int year;
    int month;
    int matrix[26][31];
} rotation;

int setMonthHeaders(int year, int month, int*** matrix) {
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

    // Allocate memory for the matrix dynamically
    (*matrix) = (int**)malloc(ROWS * sizeof(int*));

    //Allocate memory for each row
    for (int i = 0; i < ROWS; i++) {
        (*matrix)[i] = (int*)calloc(numDays, sizeof(int));
    }

    // Set the first row of the matrix with date and day headers
    for (int i = 0; i < numDays; i++) {
        (*matrix)[0][i] = i + 1;
    }

    // Set the second row of the matrix with day indices
    for (int i = 0; i < numDays; i++) {
        int day = (timeinfo.tm_wday + 3) % 7;
        (*matrix)[0][numDays + i] = day;
        timeinfo.tm_mday++;
        mktime(&timeinfo);
    }

    return numDays;
}


int day_in_arr (int * arr, int day) {

    if (day >= 0 && day < 7) {
        if (arr[day] == 1) {
            return 1;
        }
    }

    return 0;
}

void matrixToString(int** matrix, int rows, int cols, char** result) { //WIP auxiliary function to serialize the matrices into a db field; not completed
    int i, j;
    int length = 0;

    // Calculate the total length of the resulting string
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int element = matrix[i][j];
            int digits = 0;
            if (element == 0) {
                digits = 1;
            } else {
                while (element != 0) {
                    element /= 10;
                    digits++;
                }
            }
            length += digits;

            // Add comma separator for columns except the last one
            if (j < cols - 1) {
                length++; // For the comma separator
            }
        }

        // Add semicolon separator for rows except the last one
        if (i < rows - 1) {
            length++; // For the semicolon separator
        }
    }

    length++; // For the null terminator

    // Allocate memory for the buffer dynamically
    char* buffer = (char*)malloc(length);
    if (buffer == NULL) {
        printf("Failed to allocate memory for the buffer.\n");
        return;
    }

    int index = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // Convert each element to a string and concatenate it with separators
            int element = matrix[i][j];
            int digits = 0;
            if (element == 0) {
                digits = 1;
            } else {
                int temp = element;
                while (temp != 0) {
                    temp /= 10;
                    digits++;
                }
            }

            sprintf(buffer + index, "%d", element);
            index += digits;

            // Add comma separator for columns except the last one
            if (j < cols - 1) {
                buffer[index] = ',';
                index++;
            }
        }

        // Add semicolon separator for rows except the last one
        if (i < rows - 1) {
            buffer[index] = ';';
            index++;
        }
    }

    buffer[index] = '\0'; // Add the null terminator

    *result = buffer;
}

int** compareMatrices (int m1[ROWS][COLS], int m2[ROWS][COLS], int row, int col) {
    int **result = (int**)malloc(row * sizeof(int*));
    
    for (int i = 0; i < row; i++) {
        result[i] = (int*)malloc(col * sizeof(int));
    }

    for (int i = 0; i < 2; i++) { //copy header rows
        for (int j = 0; j < col; j++) {
            result[i][j] = m1[i][j];
        }
    }

    for (int i = 2; i < row; i++) { //skip header rows
        for (int j = 0; j < col; j++) {
            if (m1[i][j] == m2[i][j] && m1[i][j] == 1) {
                result[i][j] = 1;
            } else {
                result[i][j] = 0;
        }
            }
    }

    return result;
}

int main() {
    struct dirent *de;
    DIR *dr = opendir("."); // Open the current directory
    if (dr == NULL) {
        printf("Failed to open the directory.\n");
        return 1;
    }

    int **matrix, row_len;

    rotation compare[MAX_MATRICES];
    int numMatrices = 0; // Counter for the number of matrices

    while ((de = readdir(dr)) != NULL) {
        char filename[256];
        strcpy(filename, de->d_name);
        int len = strlen(filename);

        int rot_num = 0;

        // Check if the file ends with ".csv"
        if (len > 4 && strcmp(filename + len - 4, ".csv") == 0) {
            // Extract the year, month, and user ID from the file name
            int id, year, month;
            if (sscanf(filename, "%d-%d-%d.csv", &id, &year, &month) == 3) {
                printf("Reading file: %s\n", filename);

                row_len = setMonthHeaders(year, month, &matrix);

                FILE *file = fopen(filename, "r");
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

                printf("\n");

                //printf("user id: %d, year: %d, month: %d\n", id, year, month);

                compare[numMatrices].id = id;
                compare[numMatrices].year = year;
                compare[numMatrices].month = month;

                // Copy the matrix to the array of matrices
                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < row_len; j++) {
                        compare[numMatrices].matrix[i][j] = matrix[i][j];
                    }
                }

                numMatrices++;


                fclose(file);
            }
        }
    }

    closedir(dr);

    int ** result;
    
    for (int i = 0; i < numMatrices; i++) {
        printf("Rotation: shifts for user %d for month %02d/%d\n", compare[i].id, compare[i].month, compare[i].year);
        for (int j = 0; j < row_len; j++) {
            printf("%3d ", compare[i].matrix[0][j]);
        }
        printf("\n");
        for (int j = 2; j < ROWS; j++) {
            for (int k = 0; k < row_len; k++) {
                if (compare[i].matrix[j][k] == 1) {
                    printf("[X] ");
                } else {
                    printf("[ ] ");
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    result = compareMatrices(compare[0].matrix, compare[1].matrix, ROWS, row_len);

    printf("Comparison: shifts in common between users %d and %d for month %02d/%d\n", compare[0].id, compare[1].id, compare[0].month, compare[0].year);
    for (int i = 0; i < row_len; i++) {
        printf("%3d ", result[0][i]);
    }
    printf("\n");
    for (int i = 2; i < ROWS; i++) {
        for (int j = 0; j < row_len; j++) {
            if (result[i][j] == 1) {
                printf("[X] ");
            } else {
                printf("[ ] ");
            }
            //printf("%3d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}