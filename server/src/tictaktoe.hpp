#pragma once
#include <set>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

using namespace std;

void *thread_handler(void *sock);
void socket_settings(void);
bool avalible_cell_validate(int row, int col);
bool border_validate(int number);
bool game_over_validate(void);
void init_game_field(void);

