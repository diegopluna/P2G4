//
// Created by diego on 6/13/2023.
//

#ifndef P2G4_FEEDBACKS_H
#define P2G4_FEEDBACKS_H

#include <stdbool.h>
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

datetime getDate();
bool isValidDate(datetime dt);
void toggleCheckbox(bool checkboxes[], int index);
void printCheckboxes(bool checkboxes[], const char* labels[], int numOptions);
criteria getCriteria();
void freeCrit(criteria *c);
int getActivity(char** name);
void feedbackScreen();
int initializeDatabase2();
void saveFeedbackToDatabase(datetime dt, const char* activity, criteria criteria, int user_id);
int isUserExists(const char* email);
void retrieveAndPrintFeedbacks(int userId);
#endif //P2G4_FEEDBACKS_H
