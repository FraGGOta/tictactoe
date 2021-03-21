#include "tictaktoe.hpp"

char board[3][3];

char player = 'X';
char winner;

int movement = 0;

int listener, sock;

void socket_settings(void)
{
	uint32_t length;

    struct sockaddr_in addr;
    
    listener = socket(AF_INET, SOCK_STREAM, 0);

    if(listener < 0)
    {
        perror("socket");

        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    length = sizeof(addr);

    if (getsockname(listener, (struct sockaddr *)&addr, &length) < 0)
    {
    	perror("getsockname");
       	exit(3);
    }

    printf( "Server port id: [%d]\n", ntohs(addr.sin_port));

    listen(listener, 1);
}

bool avalible_cell_validate(int row, int col)
{
	if (board[row][col] == 'X' || board[row][col] == 'O') 
	{
	    return false;
	}

	return true;
}


bool border_validate(int number)
{
	if(number >= 1 && number <= 3)
	{
		return true;
	}
	else
	{
		return  false;
	}
}

bool game_over_validate()
{
    if (movement == 9)
    {
        return true;
    }

    if (movement >= 5)
    {
        if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][1] != ' ')
        {
            return true;
        }
         else if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][1] != ' ')
        {
            return true;
        }
        else if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][1] != ' ')
        {
            return true;
        }
        else if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[1][0] != ' ')
        {
            return true;
        }
        else if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[1][1] != ' ')
        {
            return true;
        }
        else if (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[1][2] != ' ')
        {
            return true;
        }
        else if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ')
        {
            return true;
        }
        else if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')
        {
            return true;
        }
    }
	return false;
}

void init_game_field()
{
    for(int i = 0 ; i < 3 ; ++i)
    {
        for(int j = 0 ; j < 3 ; ++j)
        {
            board[i][j] = ' ';
        }
    }
}
