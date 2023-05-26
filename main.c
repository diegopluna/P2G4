//
// Created by Diego Luna on 23/05/23.
//
#include <stdio.h>
#include <stdlib.h>
#include "auth.h"

int main() {
    initializeDatabase();
    int choice;
    char email[20];
    char password[20];

    User* loggedInUser = NULL;
    while (1) {
        printf("1. Sign up\n");
        printf("2. Sign in\n");
        printf("3. Logout\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp(loggedInUser);
                break;
            case 2:
                if(loggedInUser != NULL) {
                    printf("Já existe um usuário logado");
                    break;
                }
                printf("Enter email: ");
                scanf("%19s", email);
                printf("Enter password: ");
                scanf("%19s", password);
                loggedInUser = signIn(email, password);
                if (loggedInUser != NULL) {
                    printf("Logged in successfully.\n");
                }
                break;
            case 3:
                if(loggedInUser == NULL) {
                    printf("Nenhum usuario esta logado");
                    break;
                }
                logout(&loggedInUser);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        printf("\n");
    }

    return 0;
//    printf("%u", hashPassword("admin"));
    return 0;
}