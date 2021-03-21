#pragma once
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

using namespace std;

void socket_settings(void);
bool avalible_cell_validate(int row, int col);
bool border_validate(int number);
bool game_over_validate(void);
void init_game_field(void);

