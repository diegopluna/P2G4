//
// Created by Diego Luna on 23/05/23.
//
#include <stdio.h>
#include <stdlib.h>
#include "auth.h"
//int residente1=1, residente2=2, residente3=3;
//char f_resident1[150], f_resident2[150], f_resident3[150];
//int option;
int main() {
    initializeDatabase();
    int choice,choice_feedback;
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
//        //prototipo registro de feedback
//        if (loggedInUser != NULL){
//            printf("1. Registrar feedback\n");
//            printf("2. Exit\n");
//            scanf("%d",&option);
//            switch (option) {
//                case 1 : {
//                    scanf("%d", &choice_feedback);
//
//                    switch (choice_feedback) {
//                        case 1:
//                            printf("Feedback residente Numero: %d\n", residente1);
//                            fgets(f_resident1, 150, stdin);
//                            break;
//                        case 2:
//                            printf("Feedback residente Numero: %d\n", residente2);
//                            fgets(f_resident2, 150, stdin);
//                            break;
//                        case 3:
//                            printf("Feedback residente Numero: %d\n", residente3);
//                            fgets(f_resident3, 150, stdin);
//                            break;
//                    }
//                    break;
//                }
//                case 2: {
//                    exit(1);}
//            }
//        }
    }
     //printf("%u", hashPassword("admin"));
    return 0;
}
