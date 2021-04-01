#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

typedef struct sockets
{
	vector<int> *clients;
    vector<int> *opt_servs;
}sockets;

int main_server_socket_settings(uint16_t port);
int opt_server_socket_settings(uint16_t port);
void listen_main_server_by_opt_server(int sock);
void *opt_server_handler(void *socks);
void *main_server_handler(void *socks);
bool avalible_cell_validate(int row, int col);
bool border_validate(int number);
char game_over_validate(void);
void init_game_field(void);

void check_client(int sock);

