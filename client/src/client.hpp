#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define MAIN_PORT 3425

int socket_settings(uint16_t port);
int reconnect_from_choosing_sign_stage(int i, int *sock, char *sign);
int reconnect_from_waiting_choosing_sign_stage(int i, int *sock, char *sign);
int reconnect_from_waiting_making_movement_stage(int i, int *sock, char *sign);
int reconnect_from_making_movement_stage(int i, int *sock, char *sign);
void client_handler(int sock);
void init_game_field(void);
void fill_game_field(int i);
void print_game_board(void);
bool check_server(int sock);
