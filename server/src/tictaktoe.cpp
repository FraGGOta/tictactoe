#include "tictaktoe.hpp"

char board[3][3];

int movement;

int listener;

vector<char> signs;

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

void *thread_handler(void *clients)
{
    char sign;

    char current_player = 'X';

    vector<int> new_clients = *(vector<int> *) clients;

    int curr_sock = new_clients[new_clients.size() - 1];

    int other_sock;

    bool is_avl_sign = false;

    if (new_clients.size() % 2)
        other_sock = curr_sock + 1;
    else if(!(new_clients.size() % 2))
        other_sock = curr_sock - 1;

    do //Coosing the sign
    {
        recv(curr_sock, &sign, sizeof(sign), 0);

        if (!(signs.size() % 2))
        {
            is_avl_sign = true;
            signs.push_back(sign);
            send(curr_sock, &is_avl_sign, sizeof(is_avl_sign), 0);
        }
        else if (signs[signs.size() - 1] == sign)
        {
            is_avl_sign = false;
            send(curr_sock, &is_avl_sign, sizeof(is_avl_sign), 0);
        }
        else
        {
            is_avl_sign = true;
            signs.push_back(sign);
            send(curr_sock, &is_avl_sign, sizeof(is_avl_sign), 0);
        }
    }
    while(!is_avl_sign);

    init_game_field();

    movement = 0;

    while(signs.size() % 2) //Waiting for opponent
    {
        sleep(1);
    }
    bool begin_game = true;
    send(curr_sock, &begin_game, sizeof(bool), 0);

    char not_over = -1;
    send(curr_sock, &not_over, sizeof(char), 0); //Game sin't over
    
    //Send empty move to player to start the game
    int row = -1, col = -1;
    if (sign == current_player)
    {
        char winner = -1;
        send(curr_sock, &winner, sizeof(char), 0);
        send(curr_sock, &row, sizeof(row), 0);
        send(curr_sock, &col, sizeof(col), 0);
    }
    
    while(1) //Main game
    {
        bool is_val_1 = false;
        bool is_val_2 = false;
        do //Getting move
        {
            recv(curr_sock, &row, sizeof(row), 0);
            recv(curr_sock, &col, sizeof(col), 0);

            if (row == 0 && col == 0)
                return 0;
            
            is_val_1 = border_validate(row) && border_validate(col);
            is_val_2 = is_val_1 ? avalible_cell_validate(row - 1, col - 1) : true;

            send(curr_sock, &is_val_1, sizeof(is_val_1), 0);
            send(curr_sock, &is_val_2, sizeof(is_val_2), 0);
        }
        while(!is_val_1 || !is_val_2);

        board[row - 1][col - 1] = sign;

        ++movement;

        char winner = game_over_validate(); //Check if game ended
        send(curr_sock, &winner, sizeof(char), 0);

        send(other_sock, &winner, sizeof(char), 0);
        send(other_sock, &row, sizeof(row), 0);
        send(other_sock, &col, sizeof(col), 0);

        if (winner != -1)
            break;

        current_player == 'X' ? current_player = 'O' : 'X';
    }

    return 0;
}

bool avalible_cell_validate(int row, int col)
{
	if (board[row][col] == 'X' || board[row][col] == 'O')
	    return false;
	return true;
}

bool border_validate(int number)
{
	if(number >= 1 && number <= 3)
		return true;
	else
		return  false;
}

char game_over_validate()
{
    if (movement >= 5)
    {
        if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][1] != ' ')
            return board[0][1];
        else if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][1] != ' ')
            return board[1][1];
        else if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][1] != ' ')
            return board[2][1];
        else if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[1][0] != ' ')
            return board[1][0];
        else if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[1][1] != ' ')
            return board[1][1];
        else if (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[1][2] != ' ')
            return board[1][2];
        else if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ')
            return board[1][1];
        else if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')
            return board[1][1];
    }   
    else
    {
        if (movement >= 9)
            return 0;
    }

    return -1;
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
