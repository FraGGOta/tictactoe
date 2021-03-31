#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int socket_settings(uint16_t port);
void client_handler(int sock);
void init_game_field(void);
void fill_game_field(int i);
void print_game_board(void);

bool check_server(int sock);
