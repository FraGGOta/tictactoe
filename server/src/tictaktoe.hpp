#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

using namespace std;

typedef struct sockets
{
	vector<int> *clients;
    vector<int> *opt_servs;
}sockets;

int main_server_socket_settings(void);
int opt_server_socket_settings(char const *serv_id, uint16_t serv_port);
void opt_server_handler(int sock);
void *main_server_handler(void *socks);
bool avalible_cell_validate(int row, int col);
bool border_validate(int number);
char game_over_validate(void);
void init_game_field(void);

