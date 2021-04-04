#include "tictaktoe.hpp"

char board[3][3];

char current_player;

int movement;

vector<char> signs;
set<int> dead_socks;

int server_socket_settings(char const *id, uint16_t port)
{
    int listener;

    struct sockaddr_in addr;
    struct hostent *hp;
    
    listener = socket(AF_INET, SOCK_STREAM, 0);

    if(listener < 0)
    {
        perror("socket");

        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    hp = gethostbyname(id);
    
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 2);

    return listener;
}

int client_socket_settings(char const *id, uint16_t port)
{
    struct sockaddr_in addr;
    struct hostent *hp;
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    hp = gethostbyname(id);
    
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        return -1;
    }

    return sock;
}

void check_client(int sock)
{
    int status = 1;

    recv(sock, &status, sizeof(status), 0);
    send(sock, &status, sizeof(status), 0);
}

bool check_listening_server(int sock)
{
    int status;

    send(sock, &status, sizeof(status), 0);

    int bytes_read = recv(sock, &status, sizeof(status), 0);

    if(!bytes_read)
    {
        dead_socks.emplace(sock);

        return false;
    }
    
    return true;
}

void listen_current_server(int sock)
{
    int bytes_read;
    int row, col;

    char sign;

    init_game_field();

    while(1)
    {
        check_client(sock);

        bytes_read = recv(sock, &row, sizeof(row), 0);

        if (!bytes_read)
        {
            break;
        }

        recv(sock, &col, sizeof(col), 0);
        recv(sock, &sign, sizeof(sign), 0);
        recv(sock, &movement, sizeof(movement), 0);
        recv(sock, &current_player, sizeof(current_player), 0);

        board[row - 1][col - 1] = sign;
    }
}


void *opt_server_handler(void *socks)
{
    char sign;

    int row = -1, col = -1;
    int start_game = -1;

    sockets new_socks = *(sockets *)socks;

    vector<int> new_clients = *new_socks.clients;
    vector<int> new_opt_servs;

    if (new_socks.opt_servs != nullptr)
        new_opt_servs = *new_socks.opt_servs;

    int curr_sock = new_clients[new_clients.size() - 1];

    int other_sock;

    bool is_avl_sign = false;

    if (new_clients.size() % 2)
        other_sock = curr_sock + 1;
    else if(!(new_clients.size() % 2))
        other_sock = curr_sock - 1;

    recv(curr_sock, &start_game, sizeof(start_game), 0);

    if (start_game)
        recv(curr_sock, &sign, sizeof(sign), 0);
    
    if (!movement)
        current_player = 'X';

    if (!start_game)
    { 
        do 
        {
            recv(curr_sock, &sign, sizeof(sign), 0);
            if (sign != 'X' && sign != 'O')
            {
                is_avl_sign = false;
                send(curr_sock, &is_avl_sign, sizeof(is_avl_sign), 0);
            }
            else if (!(signs.size() % 2))
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

        while(signs.size() % 2) 
        {
            sleep(1);
        }

        bool begin_game = true;
        send(curr_sock, &begin_game, sizeof(begin_game), 0);

        char not_over = -1;
        send(curr_sock, &not_over, sizeof(not_over), 0); 
        
        if (sign == current_player)
        {
            char winner = -1;
            send(curr_sock, &winner, sizeof(winner), 0);
            send(curr_sock, &row, sizeof(row), 0);
            send(curr_sock, &col, sizeof(col), 0);
        }
    }

    while(1) 
    {
        bool is_val_1 = false;
        bool is_val_2 = false;
        do 
        {
            while (current_player != sign)
            {
                sleep(1);
            }

            check_client(curr_sock);

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

        char winner = game_over_validate();

        send(curr_sock, &winner, sizeof(winner), 0);
        send(other_sock, &winner, sizeof(winner), 0);
        send(other_sock, &row, sizeof(row), 0);
        send(other_sock, &col, sizeof(col), 0);

       current_player = current_player == 'X' ? 'O' : 'X';

        if(new_socks.opt_servs != nullptr)
        {    
            for (int i : new_opt_servs)
            {
                if(dead_socks.find(i) != dead_socks.end())
                {
                    if(check_listening_server(i))
                    {
                        send(i, &row, sizeof(row), 0);
                        send(i, &col, sizeof(col), 0);
                        send(i, &sign, sizeof(sign), 0);
                        send(i, &movement, sizeof(movement), 0);
                        send(i, &current_player, sizeof(current_player), 0);
                    }
                }
            }
        }

        if (winner != -1)
            break;
    }

    return 0;
}

void *main_server_handler(void *socks)
{
    char sign;

    current_player = 'X';

    sockets new_socks = *(sockets *)socks;

    vector<int> new_clients = *new_socks.clients;
    vector<int> new_opt_servs = *new_socks.opt_servs;

    int curr_sock = new_clients[new_clients.size() - 1];

    int other_sock;

    bool is_avl_sign = false;

    if (new_clients.size() % 2)
        other_sock = curr_sock + 1;
    else if(!(new_clients.size() % 2))
        other_sock = curr_sock - 1;

    do
    {
        recv(curr_sock, &sign, sizeof(sign), 0);
        
        if (sign != 'X' and sign != 'O')
        {
            is_avl_sign = false;
            send(curr_sock, &is_avl_sign, sizeof(is_avl_sign), 0);
        }
        else if (!(signs.size() % 2))
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

    while(signs.size() % 2)
    {
        sleep(1);
    }
    bool begin_game = true;

    send(curr_sock, &begin_game, sizeof(begin_game), 0);

    char not_over = -1;
    send(curr_sock, &not_over, sizeof(not_over), 0);
    
    int row = -1, col = -1;
    if (sign == current_player)
    {
        char winner = -1;
        send(curr_sock, &winner, sizeof(winner), 0);
        send(curr_sock, &row, sizeof(row), 0);
        send(curr_sock, &col, sizeof(col), 0);
    }

    while(1)
    {
        bool is_val_1 = false;
        bool is_val_2 = false;
        do
        {
            while(current_player != sign)
            {
                sleep(1);
            }
      
            check_client(curr_sock);
             
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

        char winner = game_over_validate();
     
        send(curr_sock, &winner, sizeof(winner), 0);
        send(other_sock, &winner, sizeof(winner), 0);
        send(other_sock, &row, sizeof(row), 0);
        send(other_sock, &col, sizeof(col), 0);

        current_player = current_player == 'X' ? 'O' : 'X';

        for (int i : new_opt_servs)
        {
            if(dead_socks.find(i) != dead_socks.end())
            {
                if(check_listening_server(i))
                {
                    send(i, &row, sizeof(row), 0);
                    send(i, &col, sizeof(col), 0);
                    send(i, &sign, sizeof(sign), 0);
                    send(i, &movement, sizeof(movement), 0);
                    send(i, &current_player, sizeof(current_player), 0);
                }
            }
        }

        if (winner != -1)
            break;
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

    if (movement == 9)
        return 0;

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
