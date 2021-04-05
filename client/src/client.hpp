#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

#define MAIN_PORT 3425
#define LAST_PORT 3427
#define SERV_ID "127.0.0.1"

extern const char CLIENT_CRASH_MSG;

int socket_settings(char const *id, uint16_t port);
bool check_server(int sock);
int wait_opponent_move(int *sock, char sign);
void chose_sign(int *sock, char *sign);
void wait_opponent_sign(int *sock, char sign);
void make_move(int *sock, char sign);
void client_handler(int sock);
void init_game_field(void);
void print_sign_part(int i, int j, int row);
void print_game_board(void);