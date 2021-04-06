#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

using namespace std;

#define MAIN_PORT 3425
#define SERV_ID "127.0.0.1"

extern const char CLIENT_CRASH_MSG;

typedef struct sockets
{
	vector<int> *clients;
    vector<int> *opt_servs;
}sockets;

int server_socket_settings(char const *id, uint16_t port);
int client_socket_settings(char const *id, uint16_t port);
void listen_current_server(int sock);
void *opt_server_handler(void *socks);
void *main_server_handler(void *socks);
bool avalible_cell_validate(int row, int col);
bool border_validate(int number);
char game_over_validate(void);
void init_game_field(void);
bool check_connection(int sock, int sock1);
bool check_listening_server(int sock);
void send_msg_to_listening_server(vector<int> new_opt_servs, int row, int col, char sign, int flag);

