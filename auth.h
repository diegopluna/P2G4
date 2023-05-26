//
// Created by Diego Luna on 23/05/23.
//

#ifndef P2G4_AUTH_H
#define P2G4_AUTH_H

typedef struct {
    int uid;
    char first_name[20];
    char last_name[20];
    char email[20];
    unsigned int pass_hash;
    int user_type; // 1 para residente, 2 para preceptor e 3 para administração
} User;

int initializeDatabase();
unsigned int hashPassword(const char* password);
int signUp(User* logged_in);
User* signIn(const char* email, const char* password);
void logout(User** user);

#endif //P2G4_AUTH_H

