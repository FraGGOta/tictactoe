#include "tictaktoe.hpp"

char board[3][3];

char player = 'X';
char winner;

int movement = 0;

int listener;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_handler(void *clients)
{
    pthread_mutex_init(&mutex, NULL);

    int row, col;

    vector<int> new_clients = *(vector<int> *) clients;

    int curr_sock = new_clients[new_clients.size() - 1];

    int other_sock = (curr_sock == new_clients[0]) ? new_clients[0] + 1 : new_clients[0];

    while(1)
    {
        bool is_val_3 = false;
        bool is_val_1 = false;
        bool is_val_2 = false;
    
        int game_over = 0;

        if(game_over_validate()) 
        {
            if (movement == 9)
            {
                game_over = 2;
                
                 sleep(5);

                send(curr_sock, &game_over, sizeof(game_over), 0);
            }
            else
            {
                game_over = 1;

                sleep(5);

                send(curr_sock, &game_over, sizeof(game_over), 0);
            }
          
            init_game_field();

            movement = 0;

            break;

        }
        else
        {
            sleep(5);

            send(curr_sock, &game_over, sizeof(game_over), 0);
        }

        do
        {
            recv(curr_sock, &row, sizeof(row), 0);
            recv(curr_sock, &col, sizeof(col), 0);

            is_val_1 = border_validate(row); 
            is_val_2 = border_validate(col); 
            is_val_3 = avalible_cell_validate(row - 1, col - 1);

            send(curr_sock, &is_val_1, sizeof(is_val_1), 0);
            send(curr_sock, &is_val_2, sizeof(is_val_2), 0);
            send(curr_sock, &is_val_3, sizeof(is_val_3), 0);

        }
        while(!is_val_1 || !is_val_2  || !is_val_3);

        board[row - 1][col - 1] = player;

        sleep(5);

        send(other_sock, &row, sizeof(row), 0);
        send(other_sock, &col, sizeof(col), 0);

        player = player == 'X' ? 'O' : 'X';
    
        ++movement;
    }

     pthread_mutex_destroy(&mutex);

    return 0;
}

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

    listen(listener, 2);
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
