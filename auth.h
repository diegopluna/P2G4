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
    char pass_hash[20];
} User;

unsigned int hashPassword(const char* password);


#endif //P2G4_AUTH_H

