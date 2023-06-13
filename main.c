//
// Created by Diego Luna on 23/05/23.
//
#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include "feedbacks.h"
int residente1 = 1, residente2 = 2, residente3 = 3;
char f_resident1[150], f_resident2[150], f_resident3[150];
int option;
User *loggedInUser = NULL;

void printTerminal();
void printLoggedInTerminal();


int main() {
  initializeDatabase();
  initializeDatabase2();
  printTerminal();
  // printf("%u", hashPassword("admin"));
  return 0;
}

void printTerminal() {
    int choice, choice_feedback;
    char email[20];
    char password[20];

    while (1) {
        printf("\033[2J\033[H");
//    printf("1. Sign up\n");
        printf("1. Login\n");
        printf("2. Sair\n");
        printf("Insira sua opcao: ");
        scanf("%d", &choice);

        switch (choice) {
//    case 1:
//      signUp(loggedInUser);
//      break;
            case 1:
                if (loggedInUser != NULL) {
                    printf("Ja existe um usuario logado\n");
                    break;
                }
                printf("Email: ");
                scanf("%19s", email);
                printf("Senha: ");
                scanf("%19s", password);
                loggedInUser = signIn(email, password);
                if (loggedInUser != NULL) {
                    printf("\033[2J\033[H");
                    printf("Logado com sucesso!.\n");
                    printLoggedInTerminal();
                    // user_type = loggedInUser->user_type;
                }
                break;
            case 2:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

void printLoggedInTerminal() {
    int choice;
    while (1) {
        if(loggedInUser->user_type == 1) {
            printf("\033[2J\033[H");
            printf("1. Ver boletim\n");
            printf("2. Logout\n");
            printf("Insira sua opcao: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    retrieveAndPrintFeedbacks(loggedInUser->uid);
                    getchar();
                    break;
                case 2:
                    if (loggedInUser == NULL) {
                        printf("Nenhum usuario esta logado");
                        break;
                    }
                    logout(&loggedInUser);
                    return;
            }
        } else if (loggedInUser->user_type == 2) {
            printf("\033[2J\033[H");
            printf("1. Registrar feedback\n");
            printf("2. Logout\n");
            printf("Insira sua opcao: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    feedbackScreen();
                    getchar();
                    break;
                case 2:
                    if (loggedInUser == NULL) {
                        printf("Nenhum usuario esta logado");
                        break;
                    }
                    logout(&loggedInUser);
                    return;
            }

        } else {
            printf("\033[2J\033[H");
            printf("1. Registrar Residente/Preceptor\n");
            printf("2. Logout\n");
            printf("Insira sua opcao: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    registerUser(loggedInUser);
                    break;
                case 2:
                    if (loggedInUser == NULL) {
                        printf("Nenhum usuario esta logado");
                        break;
                    }
                    logout(&loggedInUser);
                    return;
            }
        }
    }
}

