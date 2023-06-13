//
// Created by Diego Luna on 23/05/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "sqlite3.h"
#include <assert.h>

static sqlite3* db;

static int callback(void* not_used, int argc, char** argv, char** column_names) {
    return 0;
}

int initializeDatabase() {
    int rc = sqlite3_open("users.db", &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    char* query = "CREATE TABLE IF NOT EXISTS users ("
                  "uid INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "first_name TEXT NOT NULL,"
                  "last_name TEXT NOT NULL,"
                  "email TEXT UNIQUE NOT NULL,"
                  "pass_hash INT UNSIGNED NOT NULL,"
                  "user_type INTEGER NOT NULL"
                  ");";

    char* error_msg;
    rc = sqlite3_exec(db, query, callback, 0, &error_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        exit(1);
    }

    char* select_admin_query = "SELECT uid FROM users WHERE user_type = 3 AND email = 'admin@admin.com' LIMIT 1;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, select_admin_query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Admin user already exists, no need to insert
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    char* insert_admin_query = "INSERT INTO users (first_name, last_name, email, pass_hash, user_type) "
                               "VALUES ('admin', 'admin', 'admin@admin.com', 525286959, 3);";

    rc = sqlite3_exec(db, insert_admin_query, callback, 0, 0);

    if (rc != SQLITE_OK) {
        printf("Failed to insert default admin user: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    return 0;
}

//Função que representa o hash de senha, como não queríamos utilizar bibliotecas externas que dificultam
//o desenvolvimento colaborativo, optamos por representar o hash numa função simples
unsigned int hashPassword(const char* password) {
    unsigned int hash = 0;
    int i;

    for (i = 0; password[i] != '\0'; i++) {
        hash = password[i] + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

int registerUser(User* logged_in) {


    if ( logged_in == NULL ||logged_in->user_type != 3) {
        printf("Usuario nao autorizado");
        return -1;
    }

    char first_name[20];
    char last_name[20];
    char email[20];
    char password[20];
    int user_type;

    printf("Nome: ");
    scanf("%19s", first_name);
    printf("Sobrenome: ");
    scanf("%19s", last_name);
    printf("Tipo de usuario(1: Residente| 2: Preceptor| 3: Gerencia): ");
    scanf("%d", &user_type);
    printf("Email: ");
    scanf("%19s", email);
    printf("Senha: ");
    scanf("%19s", password);
    assert(user_type >0 && user_type<4);
    unsigned int pass_hash = hashPassword(password);

    char* insert_query = "INSERT INTO users (first_name, last_name, email, pass_hash, user_type) "
                         "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insert_query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, first_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, last_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 4, pass_hash);
    sqlite3_bind_int(stmt, 5, user_type);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    printf("Usuario registrado com sucesso!.\n");
    return 0;
}

User* signIn(const char* email, const char* password) {
    char* select_query = "SELECT * FROM users WHERE email = ? AND pass_hash = ?;";
    unsigned int pass_hash = hashPassword(password);
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        exit(-1);
    }

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, pass_hash);

    User* user = NULL;

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        int uid = sqlite3_column_int(stmt, 0);
        const unsigned char* first_name = sqlite3_column_text(stmt, 1);
        const unsigned char* last_name = sqlite3_column_text(stmt, 2);
        int user_type = sqlite3_column_int(stmt, 5);

        user = malloc(sizeof(User));
        user->uid = uid;
        strncpy(user->first_name, (const char*)first_name, 20);
        strncpy(user->last_name, (const char*)last_name, 20);
        strncpy(user->email, (const char*)email, 20);
        user->pass_hash = pass_hash;
        user->user_type = user_type;

        sqlite3_finalize(stmt);
        return user;
    } else if (rc == SQLITE_DONE) {
        printf("Email ou senha invalidos.\n");
        sqlite3_finalize(stmt);
        exit(-1);
    } else {
        printf("Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        exit(-1);
    }
}

void logout(User** user) {
    free(*user);
    *user = NULL;
    printf("Deslogado com sucesso.\n");
}

