#include "TicTacToe.h"

char board[3][3];
char player = 'X';
int plays = 0;
char winner;

bool isAvailable(int row, int col)
{
	if (board[row][col] == 'X' || board[row][col] == 'O') 
	{
	    cout << "That plays has already been done" << endl;
	    return false;
	}
	return true;
}


void init()
{
	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			board[i][j] = ' ';
		}
	}
}

bool validate(int number)
{
	if(number >= 1 && number <= 3)
	{
		return true;
	}
	else
	{
		cout << "Please pick a value between 1 and 3" << endl;
		return  false;
	}
}

void fillBoard(int i)
{
	for(int j = 0 ; j < 3 ; j++)
	{
		cout << board[i][j] << "\t";
	}
	cout << endl;
}

bool gameover()
{

    if (plays > 5)
    {
        if (board [0][0] == board [0][1] && board [0][1] == board [0][2] && board [0][1] != ' ')
        {
            return true;
        }
         else if (board [1][0] == board [1][1] && board [1][1] == board [1][2] && board [1][1] != ' ')
        {
            return true;
        }
        else if (board [2][0] == board [2][1] && board [2][1] == board [2][2] && board [2][1] != ' ')
        {
            return true;
        }
        else if (board [0][0] == board [1][0] && board [1][0] == board [2][0] && board [1][0] != ' ')
        {
            return true;
        }
        else if (board [0][1] == board [1][1] && board [1][1] == board [2][1] && board [1][1] != ' ')
        {
            return true;
        }
        else if (board [0][2] == board [1][2] && board [1][2] == board [2][2] && board [1][2] != ' ')
        {
            return true;
        }
        else if (board [0][0] == board [1][1] && board [1][1] == board [2][2] && board [1][1] != ' ')
        {
            return true;
        }
        else if (board [0][2] == board [1][1] && board [1][1] == board [2][0] && board [1][1] != ' ')
        {
            return true;
        }
    }
	return false;
}

void showBoard()
{
	while(1)
	{
		if(gameover()) 
		{
			system("clear");
			cout << "\t";
			for(int i = 0 ; i < 3 ; i++)
			{
				cout << i + 1 << "\t";
			}
			cout << endl;

			for(int i = 0 ; i < 3 ; i++)
			{
				cout << i + 1 << "\t";
				fillBoard(i);
			}
			cout << endl << "Player " << winner << " win." << endl;
			return;

		}

	    winner = player == 'X' ? 'X' : 'O';
		system("clear");
		int row = 0;
		int col = 0;

		cout << "It's " << player << "'s turn" << endl << endl;
		
		cout << "\t";
		for(int i = 0 ; i < 3 ; i++)
		{
			cout << i + 1 << "\t";
		}
		cout << endl;

		for(int i = 0 ; i < 3 ; i++)
		{
			cout << i + 1 << "\t";
			fillBoard(i);
		}

		do
		{
			cin >> row >> col;
		}
		while(!validate(row) || !validate(col) || !isAvailable(row - 1, col - 1));

		board[row - 1][col - 1] = player;
		plays++;
		player = player == 'X' ? 'O' : 'X';
	}
}
