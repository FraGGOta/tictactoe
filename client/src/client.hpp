#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

void socket_settings(char const *serv_id, uint16_t serv_port);
void init_game_field(void);
void fill_game_field(int i);