//
// Created by Diego Luna on 23/05/23.
//
#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int residente1 = 1, residente2 = 2, residente3 = 3;
char f_resident1[150], f_resident2[150], f_resident3[150];
int option;
User *loggedInUser = NULL;

void printTerminal();
void printLoggedInTerminal();


int main() {
  initializeDatabase();
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
        printf("2. Logout\n");
        printf("3. Sair\n");
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
                assert(password != NULL);
                loggedInUser = signIn(email, password);
                if (loggedInUser != NULL) {
                    printf("\033[2J\033[H");
                    printf("Logado com sucesso!.\n");
                    printLoggedInTerminal();
                    assert(loggedInUser != NULL);    // user_type = loggedInUser->user_type;

                }
                break;
            case 2:
                if (loggedInUser == NULL) {
                    printf("Nenhum usuário esta logado");

                    break;
                }
                logout(&loggedInUser);
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        // prototipo registro de feedback
        //loggedInUser->user_type == 2
        if(loggedInUser != NULL && loggedInUser->user_type == 2){
            printf("1. Registrar feedback\n");
            printf("2. Exit\n");
            scanf("%d", &option);
            switch (option) {
                case 1: {
                    printf("Escolha o residente :");
                    scanf("%d", &choice_feedback);

                    switch (choice_feedback) {
                        case 1:
                            printf("Feedback residente Numero: %d\n", residente1);
                            fgets(f_resident1, 150, stdin);
                            break;
                        case 2:
                            printf("Feedback residente Numero: %d\n", residente2);
                            fgets(f_resident2, 150, stdin);
                            break;
                        case 3:
                            printf("Feedback residente Numero: %d\n", residente3);
                            fgets(f_resident3, 150, stdin);
                            break;
                    }
                    break;
                }
                case 2: {
                    exit(1);
                }
            }
        }
    }
}

void printLoggedInTerminal() {
    int choice;
    while (1) {
        if(loggedInUser->user_type == 1) {
            printf("\033[2J\033[H");
            printf("Residente");
            printf("3. Logout\n");
            printf("Insira sua opcao: ");
            scanf("%d", &choice);
            assert(choice==3);
        } else if (loggedInUser->user_type == 2) {
            printf("\033[2J\033[H");
            printf("Preceptor");
            printf("3. Logout\n");
            printf("Insira sua opcao: ");
            scanf("%d", &choice);
            assert(choice==3);
        } else {
            printf("\033[2J\033[H");
            printf("1. Registrar Residente/Preceptor\n");
            printf("2. Inserir escala\n");
            printf("3. Logout\n");
            printf("Insira sua opcao: ");
            scanf("%d", &choice);
            assert(choice > 0 && choice<4);
            switch (choice) {
                case 1:
                    registerUser(loggedInUser);
                case 3:
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