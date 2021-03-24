#include "client.hpp"

char board[3][3];

int sock;

void socket_settings(char const *serv_id, uint16_t serv_port)
{
    struct sockaddr_in addr;
    struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
  
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv_port); 
    
    hp = gethostbyname(serv_id);
    
    bcopy( hp->h_addr, &addr.sin_addr, hp->h_length);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
}

void client_handler(void)
{
    init_game_field();

    char sign;

    int row, col;
    
    bool is_val_1 = false;
    bool is_val_2 = false;
    bool is_avl_sign = true;

    system("clear");
   
    cout << "Choose X/O" << endl << endl;

    do //Coosing the sign
    {
        cin >> sign;

        sign = toupper(sign);

        send(sock, &sign, sizeof(sign), 0);

        recv(sock, &is_avl_sign, sizeof(is_avl_sign), 0);

        if (!is_avl_sign)
        {
            cout << endl << sign << " has already been choosen" << endl << endl;
        }
    }
    while(!is_avl_sign);

    cout << "Waiting for opponent" << endl;
    bool begin_game = false;
    recv(sock, &begin_game, sizeof(is_avl_sign), 0);
    system("clear");

    while(1) //Main game
    {
        system("clear");
        cout << sign << " field" << endl << endl;
        print_game_board();
        cout << endl;

        char winner = -1;
        recv(sock, &winner, sizeof(char), 0);
        if (winner != -1)
        {
            if (winner == 0)
                cout << "Draw!" << endl;
            else
                cout << winner << " won!" << endl;
            break;
        }

        cout << "Waiting for opponent" << endl;
        recv(sock, &winner, sizeof(char), 0);
        recv(sock, &row, sizeof(row), 0);
        recv(sock, &col, sizeof(col), 0);
        if (row != -1 && col != -1)
            board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';

        system("clear");
        cout << sign << " field" << endl << endl;
        print_game_board();
        cout << endl;

        if (winner != -1)
        {
            if (winner == 0)
                cout << "Draw!" << endl;
            else
                cout << winner << " won!" << endl;
            row = 0;
            col = 0;
            send(sock, &row, sizeof(row), 0);
            send(sock, &col, sizeof(col), 0);
            break;
        }
       
        do //Doing move
        {
            cin >> row >> col;
            
            send(sock, &row, sizeof(row), 0);
            send(sock, &col, sizeof(col), 0);

            if (row == 0 && col == 0)
            {
               cout << endl << "Please pick a value between 1 and 3" << endl;
               continue;
            }
        
            recv(sock, &is_val_1, sizeof(is_val_1), 0);
            recv(sock, &is_val_2, sizeof(is_val_2), 0);

            if (!is_val_1)
               cout << endl << "Please pick a value between 1 and 3" << endl;
            
            if (!is_val_2)
                cout << endl << "That move has already been done" << endl;
        }
        while(!is_val_1 || !is_val_2);

        board[row - 1][col - 1] = sign;
    }
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

void fill_game_field(int i)
{
    for(int j = 0 ; j < 3 ; ++j)
    {
        cout << board[i][j] << "\t";
    }
    cout << endl;
}

void print_game_board(void)
{
    cout << "\t";

    for(int i = 0 ; i < 3 ; ++i)
    {
        cout << i + 1 << "\t";
    }

    cout << endl;

    for(int i = 0 ; i < 3 ; ++i)
    {
        cout << i + 1 << "\t";

        fill_game_field(i);
    }
}

