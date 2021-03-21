#pragma once

#include <cstdlib>
#include <iostream>

using namespace std;

bool isAvailable(int row, int col);
void init();
bool validate(int number);
void showBoard();
bool gameOver(); 
void fillBoard(int i);

